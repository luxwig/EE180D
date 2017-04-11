#ifndef _DEBUG
#include <mraa/i2c.h>
#include "LSM9DS0.h"
#endif

#include <sys/time.h>
#define MILLION 1000000.0
#include <unistd.h>

#include "cf.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "matlab_import/get_feature_terminate.h"
#include "matlab_import/get_feature_initialize.h"

int main(int argc, const char * const argv[])
{
    get_feature_initialize();
#ifdef _DEBUG
    if (argc > 1) strcpy(fn, argv[1]);
    else{
        fprintf(stderr, "Invalid Argument\n");
        exit(-1);
    }
#endif
    signal(SIGINT, do_signal);
    pthread_t acq, pro;
    buftype* buffer = (buftype*)malloc(sizeof(buftype)*_MAX_BUF_SIZE*8);
    pthread_create(&acq, NULL, data_acq, (void*) buffer);
    pthread_create(&pro, NULL, data_pro, (void*) buffer);
    pthread_join(acq, NULL);
    pthread_join(pro, NULL);
    get_feature_terminate();
    return 0;
}

void* data_acq(void* ptr)
{
    // Decleration of data seg
    buftype* buf_ptr = (buftype*) ptr;
    int counter;    
    buftype  current_data[_DATA_ACQ_SIZE];

#ifndef _DEBUG
    
    // Decleration of 9DOF

    struct timeval now;
    mraa_i2c_context accel, gyro;
    double a_res, g_res;
    accel_scale_t a_scale;
    gyro_scale_t g_scale;
    data_t ad, gd;
    data_t Go;

    
    // Init of 9DOF
    a_scale = A_SCALE_4G;
    g_scale = G_SCALE_500DPS;

    accel = accel_init();
    set_accel_scale(accel, a_scale);
    a_res = calc_accel_res(a_scale);

    gyro = gyro_init();
    set_gyro_scale(gyro, g_scale);
    g_res = calc_gyro_res(g_scale);

    Go = calc_gyro_offset(gyro, g_res);

    fprintf(stderr, "Finish init of 9DOF...\n");
#else
    FILE* fd = fopen(fn, "r"); 
    char* line = NULL;
    size_t read, len;
         
    getline(&line, &len, fd);
    getline(&line, &len, fd);

    fprintf(stderr, "** DEBUG MODE **\n");
#endif

    counter = 0;

    while (r_flag) {
#ifndef _DEBUG
        gettimeofday(&now, NULL);
        current_data[0] = now.tv_sec + now.tv_usec/MILLION;

        ad = read_accel(accel, a_res);
        gd = read_gyro(gyro, g_res);

        gettimeofday(&now, NULL);
        current_data[1] = now.tv_sec + now.tv_usec/MILLION;
        current_data[2] = ad.x;
        current_data[3] = ad.y;
        current_data[4] = ad.z;
        current_data[5] = gd.x - Go.x;
        current_data[6] = gd.y - Go.y;
        current_data[7] = gd.z - Go.z; 
#else
        read = getline(&line, &len, fd);

        if (read == -1) { r_flag = 0; continue; }
        
        sscanf(line, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
                current_data, current_data+1, current_data+2,
                current_data+3, current_data+4, current_data+5,
                current_data+6, current_data+7);
#endif
        pthread_mutex_lock(&slock);
        memcpy(buf_ptr + ((bufpos+bufsize)%_MAX_BUF_SIZE)*_DATA_ACQ_SIZE,
               current_data, sizeof(buftype)*8);
        if (bufsize < _MAX_BUF_SIZE)
            bufsize++;
        else
            bufpos = (bufpos+1)%_MAX_BUF_SIZE;

        pthread_mutex_unlock(&slock);
        counter = (counter+1)%_WAKE_UP_SPEED;
        if (counter == 0){
            pthread_mutex_lock(&cv_lock);
            pthread_cond_signal(&cv);
            pthread_mutex_unlock(&cv_lock);
        }
        usleep(100);
    }
#ifdef _DEBUG
    fclose(fd);
#endif
    pthread_mutex_lock(&cv_lock);
    pthread_cond_signal(&cv);
    pthread_mutex_unlock(&cv_lock);
    return 0;
}


/* rotate array to the left by n elements */
void rotate(const double raw_data_buf[], double correctly_ordered[], int pos) {
    memcpy(correctly_ordered, raw_data_buf+pos * _DATA_ACQ_SIZE , (_MAX_BUF_SIZE-pos) * sizeof(buftype) * _DATA_ACQ_SIZE );
    memcpy(correctly_ordered+(_MAX_BUF_SIZE * _DATA_ACQ_SIZE-pos * _DATA_ACQ_SIZE), raw_data_buf, pos * sizeof(buftype) * _DATA_ACQ_SIZE );
}

void* data_pro(void* ptr)
{
    int size, pos, n, new_seg_num;
    n = 0;
    buftype* data_buf = (buftype*) ptr,
           * correct_data_buf =
           (buftype*)malloc(_MAX_BUF_SIZE*sizeof(buftype)*_DATA_ACQ_SIZE);
    while (r_flag){ 
        pthread_mutex_lock(&cv_lock);
        pthread_cond_wait(&cv,&cv_lock);
        pthread_mutex_unlock(&cv_lock);
        pthread_mutex_lock(&slock);
        pos = bufpos; size = bufsize;
        rotate(data_buf, correct_data_buf, pos);
        fprintf(stderr, "pos: %d\tsize: %d",pos, size);
        pthread_mutex_unlock(&slock);
        /* insert here*/
        /*
        sprintf(buffer, "%d.csv", n);
        fprintf(stderr, "Write to %s\n",buffer);
        fn = fopen(buffer,"w");
        for (i = 0; i<size; i++)
            fprintf(fn,"%lf\t%lf\t%lf\t%lf\n",correct_data_buf[i*_DATA_ACQ_SIZE+1],
                                   correct_data_buf[i*_DATA_ACQ_SIZE+2],
                                   correct_data_buf[i*_DATA_ACQ_SIZE+3],
                                   correct_data_buf[i*_DATA_ACQ_SIZE+4]);
        n++;
        fclose(fn);
        */
        MoType result[_SBUFFER];
        classify_segments(correct_data_buf, pos, size, result, &new_seg_num);
    }
    return 0;
}   


