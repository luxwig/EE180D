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

    if (seg!=NULL && seg_num!=NULL)
    {
        *seg_num = pos->size[0];
        for (j = 0; j < pos->size[0];j++)
                seg[j] = (int)(pos->data[j]);
    }
    //iterate through f
    int seg_iterator = 0;

    for (j = 0; j < features->size[0]; j++) {
        for (k = 0; k < 4; k++) 
            f[*f_num*5+k] = features->data[get_index(j,k,features->size[0])];
        //use seg array to get length of interval //can we make this the interval length? 
        f[*f_num*5+4] = seg[seg_iterator + 1] - seg[seg_iterator++];
        f[*f_num*5+5] = fntype;
        fprintf(stderr, "\t%lf %lf %lf %lf %lf %lf\n", 
            f[*f_num*5], f[*f_num*5+1], f[*f_num*5+2], f[*f_num*5+3], f[*f_num*5+4], f[*f_num*5+5]);
        (*f_num)++;
    }
/*
    if (seg!=NULL && seg_num!=NULL)
    {
        *seg_num = pos->size[0];
        for (j = 0; j < pos->size[0];j++)
                seg[j] = (int)(pos->data[j]);
    }
*/

    emxDestroyArray_real_T(pos); 
    emxDestroyArray_real_T(features);
    emxDestroyArray_real_T(r);
    emxDestroyArray_real_T(m);

}



MoType test_cl(double* features, const MoType* mo_status, int mo_status_num, int flag_ind, const char* filename)
{
    int i, max;
    double* predict = (double*)malloc(sizeof(double)*(mo_status_num + 1));
    test_from_file_double(features, filename, 1, predict);
    max = 0;
    for (i = 1; i < mo_status_num+flag_ind; i++)
        if (predict[i] > predict[max])
            max = i;
    if (max < mo_status_num)
        return mo_status[max];
    else 
        return 0;
}

void create_cl(double* features, int features_num, int seg_num, MoType* mo_types, const MoType* mo_status, int mo_status_num, int flag_ind, const char* filename)
{
    // create output array
    int i, j;
    double* output = (double*)malloc(sizeof(double)*(seg_num*(mo_status_num+1)));
    for (i = 0; i < seg_num; i++)
    {
        short flag = 0;
        for (j = 0; j < mo_status_num; j++)
            if(
                (output[i*(mo_status_num+flag_ind)+j] = 
                 mo_status[j]==(mo_types[i]&0xFFFFF0)?1:-1)==1) 
                flag = 1;
        if (flag_ind) output[i*(mo_status_num+1)+mo_status_num] = (flag==0)?1:-1;
        for (j = 0; j < features_num; j++)
            fprintf(stderr, "%lf\t", features[i*features_num+j]);
        fprintf(stderr, "\n");
        for (j = 0; j < mo_status_num+flag_ind; j++)
            fprintf(stderr,"%lf\t", output[i*(mo_status_num+flag_ind)+j]);
        fprintf(stderr, "\n");
    }
    train_from_file_double(features, output, seg_num, features_num, mo_status_num+flag_ind, filename);
    free(output);
} 



void mo_training (double* data_fm, size_t n)
{
    double *features = (double*)malloc(sizeof(double)*n*_FIRST_LEVEL_FEATURES);
    MoType* mo_types = (MoType*)malloc(sizeof(MoType)*n);  
    int i;
    
    //preprocess features and output
    for (i = 0; i < n; i++)
    {
        memcpy(features+_FIRST_LEVEL_FEATURES*i, 
                data_fm+_MATLAB_OFFSET_FIRST_LEVEL*i, 
                sizeof(double)*_FIRST_LEVEL_FEATURES);
        mo_types[i] = (int)data_fm[i*_MATLAB_OFFSET_FIRST_LEVEL+4];
    }

    // train ASC_DSC
    create_cl(features, _FIRST_LEVEL_FEATURES, n, mo_types, ASC_DSC_MODEL, _ASC_DSC_SIZE, _TRUE, ASC_DSC_FN);
    // train WALK
    create_cl(features, _FIRST_LEVEL_FEATURES, n, mo_types, WALK_MODEL, _WALK_SIZE, _TRUE, WALK_FN); 
    // train FIRST_LV_ALL
    create_cl(features, _FIRST_LEVEL_FEATURES, n, mo_types, FIRST_LV_ALL_MODEL, _1ST_LV_ALL_SIZE, _FALSE, FIRST_LV_ALL_FN);
}


void mo_classfication(double* data_fm, size_t n, MoType* result)
{
    int flag = _FALSE;
    flag |= 
        ( result[_ASC_DSC_OFFSET] = test_cl(data_fm, ASC_DSC_MODEL, _ASC_DSC_SIZE, _TRUE, ASC_DSC_FN));
    
    flag |= 
        ( result[_WALK_OFFSET] = test_cl(data_fm, WALK_MODEL, _WALK_SIZE, _TRUE, WALK_FN));
    
    if (!flag) {
        result[_1ST_LV_ALL_OFFSET] =
            test_cl(data_fm, FIRST_LV_ALL_MODEL, _1ST_LV_ALL_SIZE, _FALSE,
                FIRST_LV_ALL_FN);
        switch ( (result[_1ST_LV_ALL_OFFSET] & 0xFF00)>>8) {
            case 1 : 
                result[_WALK_OFFSET] = result[_1ST_LV_ALL_OFFSET]; break;
            case 2 :
                result[_ASC_DSC_OFFSET] = result[_1ST_LV_ALL_OFFSET]; break;
        }
    }
    fprintf(stderr, "\t%d\t%d\t%d\n",result[0], result[1], result[2]);
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
    MoType segment_motion[_TOTAL_MOD_COUNT];
    int num_segments = (int)div_num - 1;
    int num_new_segments = num_segments - prev_num_segments;
    for(int i = prev_num_segments, j = 0; i < num_segments; i++, j++) {
        int start_divider = div[i];
        int end_divider = div[i+1];
        int length_of_segment = end_divider - start_divider + 1;
        //1 because single pointer
        mo_classfication(&f[_MATLAB_OFFSET_FIRST_LEVEL*i], 1, segment_motion);

        // check _WALK_OFFSET
        if(segment_motion[_WALK_OFFSET] == WALK) {
           // fprintf(stderr, "START_DIVIDER: %d\n to %d;", start_divider, end_divider);
            // if it is walk
            double *dp = (double *)malloc(sizeof(double)*(length_of_segment));
            for(int k = 0; k < length_of_segment; k++) {
                dp[k] = correct_data_buf[(start_divider+k) * _DATA_ACQ_SIZE + _ACCEL_X_OFFSET];
            }
            segment_motion[_WALK_MOD_OFFSET] = test_for_walking_speed(dp, length_of_segment, &f[5*i]);
        }
        memcpy(latestMotions+j*_TOTAL_MOD_COUNT, segment_motion, sizeof(MoType)*_TOTAL_MOD_COUNT);
    }
    prev_num_segments = num_segments > 0 ? num_segments : prev_num_segments;
    *latestMotions_num = num_new_segments;
}
