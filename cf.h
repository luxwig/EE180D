#ifndef CF_H
#define CF_H

#include "global.h"
#include "util.h"
#include <pthread.h>
#include <signal.h>

#define _MAX_BUF_SIZE   65536
#define _WAKE_UP_SPEED	7500 //change this to affect wake up time 
sig_atomic_t volatile r_flag = 1;

int bufpos  = 0;
int bufsize = 0;

pthread_mutex_t cv_lock = PTHREAD_MUTEX_INITIALIZER,
                slock = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cv = PTHREAD_COND_INITIALIZER;


void do_signal(int sig)
{
    if (sig == SIGINT)
        r_flag = 0;
    printf("BREAK\n");
}


void* data_acq(void*);
void* data_pro(void*);

typedef double buftype;

#ifdef _DEBUG
char fn[256];
#endif
#endif
