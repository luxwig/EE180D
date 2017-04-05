#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#define BUFFER 512

sig_atomic_t volatile r_flag = 1;
void do_signal(int sig)
{
    if (sig == SIGINT)
        r_flag = 0;
    printf("BREAK\n");
}
void* data_acq(void* ptr);
void* data_pro(void* ptr);
int bufpos  = 0;
int bufsize = 0;

pthread_mutex_t cv_lock = PTHREAD_MUTEX_INITIALIZER,
                slock = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
typedef int buftype;
int main()
{
    signal(SIGINT, do_signal);
    pthread_t acq, pro;
    buftype* buffer = (buftype*)malloc(sizeof(buftype)*BUFFER);
    pthread_create(&acq, NULL, data_acq, (void*) buffer);
    pthread_create(&pro, NULL, data_pro, (void*) buffer);
    pthread_join(acq, NULL);
    pthread_join(pro, NULL);
}

void* data_acq(void* ptr)
{
    int i_c = 0;
    buftype* b = (buftype*) ptr;
    
    while (r_flag) {
        pthread_mutex_lock(&slock);
        if (bufsize < BUFFER)
            bufsize++;
        else
            bufpos = (bufpos+1)%BUFFER;
        b[(bufpos+bufsize)%BUFFER] = i_c++;
        pthread_mutex_unlock(&slock);
        pthread_mutex_lock(&cv_lock);
        pthread_cond_signal(&cv);
        pthread_mutex_unlock(&cv_lock);
        i_c %= 300;
        usleep(100);
    }
    pthread_mutex_lock(&cv_lock);
    pthread_cond_signal(&cv);
    pthread_mutex_unlock(&cv_lock);

    return 0;
}

void* data_pro(void *ptr)
{
    int size, pos, i;
    buftype* b = (buftype*) ptr,
             b_l[BUFFER];
    while (r_flag){ 
        pthread_mutex_lock(&cv_lock);
        pthread_cond_wait(&cv,&cv_lock);
        pthread_mutex_unlock(&cv_lock);
        pthread_mutex_lock(&slock);
        pos = bufpos; size = bufsize;
        memcpy((void*)b_l, (void*)b, sizeof(buftype)*BUFFER);
        pthread_mutex_unlock(&slock);
        /* insert here*/
        
        for (i = bufpos; i<bufpos+bufsize; i++)
            if (b_l[i%BUFFER] == 299)
                printf("%d\t", i%BUFFER);
        printf("\n");
    }
    return 0;
}
