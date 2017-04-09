#include <mraa/i2c.h>
#include <sys/time.h>
#include "LSM9DS0.h"
#define MILLION 1000000.0
#include <unistd.h>

#include "cf.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

int main(int argc, const char * const argv[])
{
    signal(SIGINT, do_signal);
    pthread_t acq, pro;
    buftype* buffer = (buftype*)malloc(sizeof(buftype)*_MAX_BUF_SIZE);
    pthread_create(&acq, NULL, data_acq, (void*) buffer);
    pthread_create(&pro, NULL, data_pro, (void*) buffer);
    pthread_join(acq, NULL);
    pthread_join(pro, NULL);
    return 0;
}

void* data_acq(void* ptr)
{
    // Decleration of 9DOF
    struct timeval now;

    mraa_i2c_context accel, gyro;
    double a_res, g_res;
    accel_scale_t a_scale;
    gyro_scale_t g_scale;
    data_t ad, gd;
    data_t Go;

    // Decleration of data seg
    buftype* buf_ptr = (buftype*) ptr;
    int counter;    
    buftype  current_data[_DATA_ACQ_SIZE];

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

    counter = 0;
    fprintf(stderr, "Finish init of 9DOF...\n");
    while (r_flag) {
        pthread_mutex_lock(&slock);
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
    pthread_mutex_lock(&cv_lock);
    pthread_cond_signal(&cv);
    pthread_mutex_unlock(&cv_lock);
    return 0;
}



void rotate(const double *raw_data_buf, double *correctly_ordered, int size, int pos)
{
    int i, j = 0, remaining_elements = size;
    //copy from beginning
    for(i = pos; i < _MAX_BUF_SIZE && i < (pos + size); i++, j++, remaining_elements--) {
        correctly_ordered[j] = raw_data_buf[i];
    }
    //if i reaches max_buf_size, copy remaining items from beginning
    i = 0;
    while(remaining_elements-- > 0) {
        correctly_ordered[j++] = raw_data_buf[i++];
    }    
}

void* data_pro(void* ptr)
{
    FILE* fn;
    char buffer [33];
    int size, pos, i,n;
    n = 0;
    buftype* data_buf = (buftype*) ptr,
             correct_data_buf[_MAX_BUF_SIZE*8];
    while (r_flag){ 
        pthread_mutex_lock(&cv_lock);
        pthread_cond_wait(&cv,&cv_lock);
        pthread_mutex_unlock(&cv_lock);
        pthread_mutex_lock(&slock);
        pos = bufpos; size = bufsize;
        rotate(data_buf, correct_data_buf, size, pos);
        fprintf(stderr, "pos: %d\tsize: %d",pos, size);
        pthread_mutex_unlock(&slock);
        /* insert here*/
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
    }
    return 0;
}   


