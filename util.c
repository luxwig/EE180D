#define get_index(X,Y,N) X+N*Y
#define MOTION_N_FEATURES 4
#define WALK_N_FEATURES (5+4) //second level + first level
#define WALK_N_OUTPUTS (4) 
#define WALK_MAXIMA_INDEX 0
#define WALK_MINIMA_INDEX 1
#define WALK_PERIOD_INDEX 2

#include "global.h"
#include "util.h"
#include "matlab_import/rt_nonfinite.h"
#include "matlab_import/get_feature.h"
#include "matlab_import/get_feature_terminate.h"
#include "matlab_import/get_feature_emxAPI.h"
#include "matlab_import/get_feature_initialize.h"
#include "FANN/fann_util.h"

#include <stdio.h>
#include <stdlib.h>
void read_from_file(const char * filename, double * buffer, size_t* n) {
    FILE* file = fopen(filename, "r"); 
    char* line = NULL;
    size_t read, len;
    //must skip the first two lines of the code
    getline(&line, &len, file);
    getline(&line, &len, file);
    *n = 0;
    while((read = getline(&line, &len, file)) != -1) {
        sscanf(line, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
                buffer + *n * _DATA_ACQ_SIZE + _TIMESTAMP_BEFORE_OFFSET, 
                buffer + *n * _DATA_ACQ_SIZE + _TIMESTAMP_AFTER_OFFSET, 
                buffer + *n * _DATA_ACQ_SIZE + _ACCEL_X_OFFSET,
                buffer + *n * _DATA_ACQ_SIZE + _ACCEL_Y_OFFSET, 
                buffer + *n * _DATA_ACQ_SIZE + _ACCEL_Z_OFFSET,                
                buffer + *n * _DATA_ACQ_SIZE + _GYRO_X_OFFSET,
                buffer + *n * _DATA_ACQ_SIZE + _GYRO_Y_OFFSET,
                buffer + *n * _DATA_ACQ_SIZE + _GYRO_Z_OFFSET);
        *n = *n + 1;
    }
    fclose(file);
}

/*
    fn: filename
    f: actual data
    f_num: number of deatures
    seg: indexes of dividers
    segm_num: number of segments
    fntype: filename type

    places features contiguously into f for the first level classification

*/

void segmentation(const double* data_buf, const int data_buf_size, double* f, size_t* f_num, int* seg, size_t* seg_num, int fntype)
{
    int j, k, n;
    double* data_r = (double*)malloc(sizeof(double)*_BUFFER*2);

    emxArray_real_T *r;
    emxArray_real_T *pos;
    emxArray_real_T *features;
    emxArray_real_T *m;
    *f_num = 0;

    n = data_buf_size-1;

    for (k = 0; k < n; k++)
        for (j = 1 ; j < 8; j++)
            data_r[get_index(k,(j-1),n)] = data_buf[k*8+j];
    
    emxInitArray_real_T(&r, 2);
    emxInitArray_real_T(&features, 2);
    emxInitArray_real_T(&pos, 1);
    
    m = emxCreateWrapper_real_T(data_r,n,7);

    get_feature(m, pos, r, features);
    
    fprintf(stderr, "%d %d\n", r->size[0], r->size[1]);
    
    for (j = 0; j < features->size[0]; j++) {
        for (k = 0; k < 4; k++) 
            f[*f_num*5+k] = features->data[get_index(j,k,features->size[0])];
        f[*f_num*5+4] = fntype;
        fprintf(stderr, "\t%lf %lf %lf %lf\n", 
            f[*f_num*5], f[*f_num*5+1], f[*f_num*5+2], f[*f_num*5+3]);
        (*f_num)++;
    }

    if (seg!=NULL && seg_num!=NULL)
    {
        *seg_num = pos->size[0];
        for (j = 0; j < pos->size[0];j++)
                seg[j] = (int)(pos->data[j]);
    }

    emxDestroyArray_real_T(pos); 
    emxDestroyArray_real_T(features);
    emxDestroyArray_real_T(r);
    emxDestroyArray_real_T(m);

}


MoType mo_classfication(double* data_fm, size_t n, MoType fntype)
{
    double* input, *output;
    int j, k;
    if (fntype == TRAINING) 
    {
        input  = (double*)malloc(sizeof(double)*n*4);
        output  = (double*)malloc(sizeof(double)*n*3);

        const double output_type[3][3]={{1,-1,-1},{-1,1,-1},{-1,-1,1}};
        for (j = 0; j < n; j++) {
            memcpy(input+4*j,data_fm+5*j, sizeof(double)*4);
            if (((int)data_fm[j*5+4]&0xF0) != 0xF0)
                memcpy(&output[j*3], output_type[0], sizeof(double)*3);
            else
                memcpy(&output[j*3], output_type[(int)data_fm[j*5+4]&0x00F], sizeof(double)*3);
            for (k = 0; k < 4; k++)
                fprintf(stderr,"%f ", input[j*4+k]);
            fprintf(stderr,"\n");
            for (k = 0; k < 3; k++)
                fprintf(stderr,"%f ", output[j*3+k]);
            fprintf(stderr,"\n");
        }
    
  
        train_from_file_double(input, output, n, MOTION_N_FEATURES, 3, _MO_NEURAL_NETWORK);
        return TRAINING;
    }
    else
    {
        double predict[3];
        test_from_file_double(data_fm, _MO_NEURAL_NETWORK, 1, predict);
        //1 parameter: 3 features: [max, min, period, .....
        //2 parameter: 


        fprintf(stderr, "\t%f\t%f\t%f\n",predict[0], predict[1], predict[2]);
        k = predict[0]>predict[1]?0:1;
        k = predict[k]>predict[2]?k:2;
        fprintf(stderr,"\t%d\n", k+1);
        if (k == 0) return 0;
        return (k|0xF0);
    }    
}



void train_walk_neural_network(TrainingData all_file_data[], int nFiles) {
    float *input;
    float *output;
    unsigned int num_data, num_input, num_output;
    input = output = NULL;
    num_data = 0;
    num_input = WALK_N_FEATURES;
    num_output = WALK_N_OUTPUTS;

    for(int i = 0; i < nFiles; i++) {
        if ((all_file_data[i].m_type & 0xF0) >> 4) continue;
        num_data += (all_file_data[i].m_num_divider - 1);
    }

    input = (float *)malloc(sizeof(float)*WALK_N_FEATURES*num_data);
    output = (float *)malloc(sizeof(float)*WALK_N_OUTPUTS*num_data);
    int n = 0;
    for(int i = 0; i < nFiles; i++){
        if ((all_file_data[i].m_type & 0xF0) >> 4) continue;
        int m_num_divider = all_file_data[i].m_num_divider;
        for(int j = 1; j< m_num_divider; j++) {
            int start = all_file_data[i].m_divider[j-1];
            int end = all_file_data[i].m_divider[j];
            float* convert_m_data = (float*)malloc(sizeof(float)*all_file_data[i].m_num_data);
            for (int k = 0; k < all_file_data[i].m_num_data; k++){
                convert_m_data[k] = all_file_data[i].m_data[k*7+1];
            }
            float maxima = w_maxima_seg(convert_m_data, start, end);
            float minima = w_minima_seg(convert_m_data, start, end);
            float period = end - start + 1;
            input[n*WALK_N_FEATURES] = maxima;
            input[n*WALK_N_FEATURES+1] = minima;
            input[n*WALK_N_FEATURES+2] = period;
            input[n*WALK_N_FEATURES+3] = w_mean_float(convert_m_data, period);
            input[n*WALK_N_FEATURES+4] = w_RMS_seg(convert_m_data, period);

            input[n*WALK_N_FEATURES+5] = all_file_data[i].m_1st_feature[0+(j-1)*_MATLAB_OFFSET_FIRST_LEVEL];
            input[n*WALK_N_FEATURES+6] = all_file_data[i].m_1st_feature[1+(j-1)*_MATLAB_OFFSET_FIRST_LEVEL];
            input[n*WALK_N_FEATURES+7] = all_file_data[i].m_1st_feature[2+(j-1)*_MATLAB_OFFSET_FIRST_LEVEL];
            input[n*WALK_N_FEATURES+8] = all_file_data[i].m_1st_feature[3+(j-1)*_MATLAB_OFFSET_FIRST_LEVEL];
            
            output[n*WALK_N_OUTPUTS] = (all_file_data[i].m_type == WALK1)*2-1;
            output[n*WALK_N_OUTPUTS+1] = (all_file_data[i].m_type == WALK2)*2-1;
            output[n*WALK_N_OUTPUTS+2] = (all_file_data[i].m_type == WALK3)*2-1;
            output[n*WALK_N_OUTPUTS+3] = (all_file_data[i].m_type == WALK4)*2-1;
            n++;
        }
    }
    train_from_file_float(input, output, num_data, num_input, num_output, _WALK_NEURAL_NETWORK);
}

MoType test_for_walking_speed(double *segment,int length, double* first_level_features) 
{
    double maxima_x_accel = w_maxima_double_seg(segment, 0, length);
    double minima_x_accel = w_minima_double_seg(segment, 0, length);
    double period_x_accel = (double)length;
    double mean_x_accel = (double)w_mean(segment,length);
    double RMS_x_accel = w_RMS_seg_double(segment,length);

    double features[] = {
        maxima_x_accel, 
        minima_x_accel, 
        period_x_accel, 
        mean_x_accel, 
        RMS_x_accel,
        first_level_features[0],
        first_level_features[1],
        first_level_features[2],
        first_level_features[3]
    };
    double result[4];
    test_from_file_double(features, _WALK_NEURAL_NETWORK, 1, result);
    int maximum = 0;
    for(int i = 0 ; i < WALK_N_OUTPUTS; i++) {
        if (result[i] > result[maximum]) {
            maximum = i;
        }
    }
    return (maximum+1);
}


static int prev_num_segments = 0;
/*
    The function returns an array of the latest motions by taking the difference
    of the new size of the buffer with that of the old size, and accessing the 
    values in the new segments.

    @param correct_data_buf : array of metric data
    @param pos: position earliest data point
    @param size: size of buff, will reach MAX_BUF_SIZE and increase no more
*/
void classify_segments(double* correct_data_buf, int pos, int size, MoType* latestMotions, int* latestMotions_num) {
    /*
    @ data_buf -> actual data containing all metrics, segmentation will handle gyro dirctly
    @ data_buf_size -> # of data points
    @ f -> [will be modified], turning to array of features * the number of things
    @ seg -> [will be modified], actual dividers
    @ seg_num -> [will be modified], number of dividers
    @ fntype -> TEST macro
    */

    double* f = (double*)malloc(sizeof(double*)*(_BUFFER));;
    size_t f_num;
    int *div = (int*)malloc(sizeof(int)*_SBUFFER);
    size_t div_num;
    int fntype = TEST;
    segmentation(correct_data_buf, size, f, &f_num, div, &div_num, fntype);

    /* 
        @param num_segments: number of total segments
        @param num_new_segments: number of new segments
        if very first motion, ignore last divider
            otherwise ignore first and last divider
    */
    //int num_segments = (prev_num_segments > 0) ? ((int)div_num - 2) : ((int)div_num - 1); 
    int num_segments = (int)div_num - 1;
    int num_new_segments = num_segments - prev_num_segments;
    for(int i = prev_num_segments, j = 0; i < num_segments; i++, j++) {
        int start_divider = div[i];
        int end_divider = div[i+1];
        int length_of_segment = end_divider - start_divider + 1;
        //1 because single pointer
        MoType segment_motion = mo_classfication(&f[5*i], 1, TEST);
        if(segment_motion == TRAINING) {
           // fprintf(stderr, "START_DIVIDER: %d\n to %d;", start_divider, end_divider);
            double *dp = (double *)malloc(sizeof(double)*(length_of_segment));
            for(int k = 0; k < length_of_segment; k++) {
                dp[k] = correct_data_buf[(start_divider+k) * _DATA_ACQ_SIZE + _ACCEL_X_OFFSET];
            }
            segment_motion = test_for_walking_speed(dp, length_of_segment, &f[5*i]);
        }
        latestMotions[j] = segment_motion;
    }
    prev_num_segments = num_segments > 0 ? num_segments : prev_num_segments;
    *latestMotions_num = num_new_segments;
}
