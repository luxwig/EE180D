#include "customio.h"
#include <stdio.h>
#include "string.h"

#ifndef _DEBUG
#include <mraa/i2c.h>
#include "LSM9DS0.h"
#endif

#include <sys/time.h>
#define MILLION 1000000.0
#define SLEEPTIME 100
#include <unistd.h>



void MoType_to_str(MoType t, char* str, int type)
{
    switch (t&_MASK_LV1)
    {
        case WALK:
            strcpy(str, "WALK");
            break;
        case RUN:
            strcpy(str, "RUN");
            break;
        case ASC:
            strcpy(str, "ASC");
            break;
        case DSC:
            strcpy(str, "DSC");
            break;
        case TURNL:
            strcpy(str, "TURNL");
            break;
        case TURNR:
            strcpy(str, "TURNR");
            break;
        case JUMP:
            strcpy(str, "JUMP");
            break;
        case NONE:
            break;
        default:
            strcpy(str, "UDEF");
    }
    if (type&0xF0)
    {
        char digit[3];
        int len = strlen(str);
        if (t&0xF)
        {
            sprintf(digit, "%d", t&0xF);
            strcpy(str+len, digit);
        }
    }
    if (type&0x0F)
        strcpy(str+strlen(str), " ");
}

void get_motion_str_from_array(MoType* result, char* str)
{
    if (result[_WALK_RUN_OFFSET] != 0)
        MoType_to_str(result[_WALK_RUN_MOD_OFFSET],
                      str, _IO_ALL_SPACE);
    if (result[_ASC_DSC_OFFSET] != 0)
        MoType_to_str(result[_ASC_DSC_MOD_OFFSET],
                      str+strlen(str), _IO_ALL_SPACE);
    MoType_to_str(result[_TURNR_TURNL_OFFSET], str+strlen(str), _IO_ALL_SPACE);
    MoType_to_str(result[_JUMP_OFFSET], str+strlen(str), _IO_ALL_SPACE);
}


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void get_data(const char* cmd_text, int rate, double* buffer, int* buf_size)
{
    signal(SIGINT, do_signal);
    printf("%s\n",cmd_text);
    printf("PRESS ENTER TO CONTINUE...\n");
    printf("PRESS CTRL+C TO BREAK...\n");
    getchar();
    buftype  current_data[_DATA_ACQ_SIZE];
#ifndef _DEBUG
    printf(ANSI_COLOR_YELLOW "Start to init 9DOF...");    
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

    printf("Finished.\n" ANSI_COLOR_RESET);
#else
/*    
    FILE* fd = fopen(fn, "r"); 
    char* line = NULL;
    size_t read, len;
         
    getline(&line, &len, fd);
    getline(&line, &len, fd);
*/
    fprintf(stderr, ANSI_COLOR_YELLOW "** DEBUG MODE **\n" ANSI_COLOR_RESET);

#endif
    *buf_size = 0;
    while (r_flag_io) {
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
/*        read = getline(&line, &len, fd);

        if (read == -1) { r_flag_io = 0; continue; }
        
        sscanf(line, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
                current_data, current_data+1, current_data+2,
                current_data+3, current_data+4, current_data+5,
                current_data+6, current_data+7);
*/
        int p;
        for (p = 0;p < 8; p++)
        {
            current_data[p] = p;
        }
#endif
        memcpy(buffer+(*buf_size)*8, current_data, sizeof(double)*8);
        *buf_size = *buf_size+1;
        usleep(rate);
    } 
#ifdef _DEBUG
   // fclose(fd);
#endif
 
}
