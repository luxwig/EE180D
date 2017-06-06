#ifndef CF_H
#define CF_H

#include "global.h"
#include "util.h"
#include <pthread.h>
#include <signal.h>

#define _MAX_BUF_SIZE   65536999
#define _WAKE_UP_SPEED	3000
int bufpos  = 0;
int bufsize = 0;

pthread_mutex_t cv_lock = PTHREAD_MUTEX_INITIALIZER,
                slock = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

void* data_acq(void*);
void* data_pro(void*);

typedef double buftype;

#ifdef _DEBUG
char fn[256];
#endif
#endif
