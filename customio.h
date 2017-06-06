#ifndef CUSTOMIO_H
#define CUSTOMIO_H

#include "global.h"
#include "util.h"
#include <pthread.h>
#include <signal.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define _IO_NAME_ONLY_NO_SPACE 0x00
#define _IO_NAME_ONLY_SPACE    0x0F
#define _IO_ALL_NO_SPACE       0xF0
#define _IO_ALL_SPACE          0xFF

typedef double buftype;

char* MoType_to_str(MoType t, char* str, int type);
void get_motion_str_from_array(MoType* result, char* str);

void get_data(MoType cmd_text, char* text, int speed, int rate, double* buffer, int* buf_size);
static sig_atomic_t volatile r_flag_io = 1;
static void do_signal(int sig)
{
    if (sig == SIGINT)
        r_flag_io = 0;
    printf("BREAK\n");
}

#ifdef _DEBUG
char fn[256];
#endif
#endif
