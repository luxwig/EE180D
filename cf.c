#include "cf.h"
#include <mraa/i2c.h>
#include <sys/time.h>
#include "LSM9DS0.h"
#define MILLION 1000000.0

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
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
	m_scale = M_SCALE_4GS;	

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
        if (bufsize < _MAX_BUF_SIZE)
            bufsize++;
        else
            bufpos = (bufpos+1)%_MAX_BUF_SIZE;
        gettimeofday(&now, NULL);
		current_data[0] = now.tv_sec + now.tv_usec/MILLION;

		ad = read_accel(accel, a_res);
		gd = read_gyro(gyro, g_res);
		md = read_mag(mag, m_res);

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
    for(i = pos; i < MAX_BUF_SIZE && i < (pos + size); i++, j++, remaining_elements--) {
        correctly_ordered[j] = raw_data_buf[i];
    }
    //if i reaches max_buf_size, copy remaining items from beginning
    if (remaining_elements > 0) {
        i = 0;
        while(remaining_elements-- > 0) {
            correctly_ordered[j++] = raw_data_buf[i++];
        }
    }
    
}

void* data_pro(void* ptr)
{
    int size, pos, i;
    buftype* data_buf = (buftype*) ptr,
             corret_data_buf[_MAX_BUF_SIZE];
    while (r_flag){ 
        pthread_mutex_lock(&cv_lock);
        pthread_cond_wait(&cv,&cv_lock);
        pthread_mutex_unlock(&cv_lock);
        pthread_mutex_lock(&slock);
        pos = bufpos; size = bufsize;
        rotate(data_buf, corret_data_buf, size, pos);
        pthread_mutex_unlock(&slock);
        /* insert here*/
        
        for (i = bufpos; i<bufpos+bufsize; i++)
            if (b_l[i%BUFFER] == 299)
                printf("%d\t", i%BUFFER);
        printf("\n");
    }
    return 0;
}   


