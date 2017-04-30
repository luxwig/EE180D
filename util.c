#define get_index(X,Y,N) X+N*Y
//walk neural network and run neural network use same number of features

#define WALK_N_OUTPUTS _WALK_LV2_SIZE 
#define WALK_MAXIMA_INDEX 0
#define WALK_MINIMA_INDEX 1
#define WALK_PERIOD_INDEX 2

#define RUN_N_OUTPUTS _RUN_LV2_SIZE

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

//returns title of neural network associated with the specific motion 
static const char * get_neural_network_name(MoType motion) {
    switch(motion) {
        case RUN:
            return RUN_LV2_FN;
        case WALK:
            return WALK_NEURAL_NETWORK;
        case ASC:
            return ASC_LV2_FN;
        case DSC:
            return DSC_LV2_FN;
        default: //in case bad motion specified, return most generalized neural network.
            return MO_NEURAL_NETWORK;
    }
}

//returns the number of outputs of a neural network associated with the specified motion
static int get_n_neural_network_outputs(MoType motion) {
    switch(motion) {
        case RUN:
            return RUN_N_OUTPUTS;
        case WALK:
            return WALK_N_OUTPUTS;
        case ASC:
            return _ASC_LV2_SIZE;
        case DSC:
            return _DSC_LV2_SIZE;
        default: //make sure to check for failures.
            return -1;
    }
}

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

int segmentation(const double* data_buf, const int data_buf_size, double* f, size_t* f_num, int* seg, size_t* seg_num, int fntype)
{
    int j, k, n;
    double* data_r = (double*)malloc(sizeof(double)*_BUFFER*2);
    emxArray_real_T *r;
    emxArray_real_T *pos;
    emxArray_real_T *features;
    emxArray_real_T *m;
    *f_num = 0;

    n = data_buf_size-1;

    double* gyro_z = (double*)malloc(sizeof(double)*n);
    for (k = 0; k < n; k++){
        for (j = 1 ; j < 8; j++)
            data_r[get_index(k,(j-1),n)] = data_buf[k*8+j];
        gyro_z[k] = data_buf[k*8+7];
    }
    double mean = w_mean(gyro_z, n);
    double std = w_std(gyro_z,n); 
    double th = mean + std;
    double int_count[_SBUFFER]={};
    int count = -1;
    for (j = 0; j < n; j++)
    {
        // below thold
        if ((gyro_z[j]) <= th && gyro_z[j] >= mean-std)
        {
            // first time ignore
            if (count == -1) {
                while ((gyro_z[j]) <= th && gyro_z[j] >= mean-std) 
                    j++; 
                count++; 
                continue;
            }

            int_count[count] += 1;
        }
        else
        {
            if (int_count[count] != 0)
                count++;
        }
    }
    count--;
    double interval_mean = w_mean(int_count,count);
    double interval_std  = w_std(int_count,count);
    double interval_th = interval_mean + 1* interval_std;
    if (count==0)
        interval_th = 90;
    fprintf(stderr,"interval_th = %lf\n", interval_th);
    emxInitArray_real_T(&r, 2);
    emxInitArray_real_T(&features, 2);
    emxInitArray_real_T(&pos, 1);
    
    m = emxCreateWrapper_real_T(data_r,n,7);

    fprintf(stderr, "MEAN+STD:%f\n" , mean+std);
    get_feature(abs(interval_th), mean+std, m, pos, r, features);
    
    fprintf(stderr, "%d %d\n", r->size[0], r->size[1]);

    if (seg!=NULL && seg_num!=NULL)
    {
        *seg_num = pos->size[0];
        for (j = 0; j < pos->size[0];j++)
                seg[j] = (int)(pos->data[j]);
    }
    //iterate through f
    for (j = 0; j < pos->size[0]; j++)
    {
        fprintf(stderr,"%d,",(int)(pos->data[j]));
    }
    fprintf(stderr,"\n");
    int seg_iterator = 0;
    if (features->size[0] == 0 || features->size[1] == 0)
        return _FALSE;
    for (j = 0; j < features->size[0]; j++) {
        for (k = 0; k < 4; k++) 
            f[*f_num*_MATLAB_OFFSET_FIRST_LEVEL+k] = features->data[get_index(j,k,features->size[0])];
        //use seg array to get length of interval //can we make this the interval length? 
        f[*f_num*_MATLAB_OFFSET_FIRST_LEVEL+4] = seg[seg_iterator + 1] - seg[seg_iterator];
	seg_iterator++;
        f[*f_num*_MATLAB_OFFSET_FIRST_LEVEL+5] = fntype;
        for (k = 0; k < _MATLAB_OFFSET_FIRST_LEVEL; k++)
            fprintf(stderr, "\t%lf", f[*f_num*_MATLAB_OFFSET_FIRST_LEVEL+k]);
        fprintf(stderr,"\n"); 
        (*f_num)++;
    }
    emxDestroyArray_real_T(pos); 
    emxDestroyArray_real_T(features);
    emxDestroyArray_real_T(r);
    emxDestroyArray_real_T(m);
    return _TRUE;
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

void create_cl(double* features, int features_num, int seg_num, MoType* mo_types, const MoType* mo_status, int mo_status_num, int flag_ind, int mask, const char* filename)
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
                 mo_status[j]==(mo_types[i]&mask)?1:-1)==1) 
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


void mo_training(double* data_fm, size_t n)
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
        mo_types[i] = (int)data_fm[i*_MATLAB_OFFSET_FIRST_LEVEL+_MATLAB_OFFSET_FIRST_LEVEL-1];
    }

    // train ASC_DSC
    create_cl(features, _FIRST_LEVEL_FEATURES, n, mo_types, ASC_DSC_MODEL, _ASC_DSC_SIZE, _TRUE, _MASK_LV1, ASC_DSC_FN);
    // train WALK
    create_cl(features, _FIRST_LEVEL_FEATURES, n, mo_types, WALK_RUN_MODEL, _WALK_RUN_SIZE, _TRUE, _MASK_LV1, WALK_RUN_FN); 
    // train FIRST_LV_ALL
    create_cl(features, _FIRST_LEVEL_FEATURES, n, mo_types, FIRST_LV_ALL_MODEL, _1ST_LV_ALL_SIZE, _FALSE, _MASK_LV1, FIRST_LV_ALL_FN);
}


void mo_classfication(double* data_fm, size_t n, MoType* result)
{
    int flag = _FALSE;
    flag |= 
        ( result[_ASC_DSC_OFFSET] = test_cl(data_fm, ASC_DSC_MODEL, _ASC_DSC_SIZE, _TRUE, ASC_DSC_FN));
    
    flag |= 
        ( result[_WALK_RUN_OFFSET] = test_cl(data_fm, WALK_RUN_MODEL, _WALK_RUN_SIZE, _TRUE, WALK_RUN_FN));
    
    if (!flag) {
        result[_1ST_LV_ALL_OFFSET] =
            test_cl(data_fm, FIRST_LV_ALL_MODEL, _1ST_LV_ALL_SIZE, _FALSE,
                FIRST_LV_ALL_FN);
        switch ( (result[_1ST_LV_ALL_OFFSET] & 0xFF00)>>8) {
            case 1 : 
                result[_WALK_RUN_OFFSET] = result[_1ST_LV_ALL_OFFSET]; break;
            case 2 :
                result[_ASC_DSC_OFFSET] = result[_1ST_LV_ALL_OFFSET]; break;
        }
    }
    fprintf(stderr, "\t%d\t%d\t%d\n",result[0], result[1], result[2]); 
}


void train_lv2_neural_network(TrainingData all_file_data[], int nFiles, MoType mo_type_lv1,int input_size, int output_size, const MoType* model , const char* fn)
{
    int i;
    int num_data = 0;
    for (i = 0; i < nFiles; i++) 
        if (_GET_MO_TYPE(all_file_data[i].m_type) == mo_type_lv1)
            num_data += (all_file_data[i].m_num_divider - 1);
    double* input = (double *)malloc(sizeof(double)*input_size*num_data);
    int n = 0; 
    MoType* mo_types = (MoType*)malloc(sizeof(MoType)*num_data);
    for(int i = 0; i < nFiles; i++){
        if (_GET_MO_TYPE(all_file_data[i].m_type) != mo_type_lv1) continue;
        int m_num_divider = all_file_data[i].m_num_divider;
        for(int j = 1; j< m_num_divider; j++) {
            int start = all_file_data[i].m_divider[j-1];
            int end = all_file_data[i].m_divider[j];
            
            double* convert_m_data = (double*)malloc(sizeof(double)*all_file_data[i].m_num_data);
            
            for (int k = 0; k < all_file_data[i].m_num_data; k++){
                convert_m_data[k] = all_file_data[i].m_data[k*7+1];
            }
            
            double maxima = w_maxima_double_seg(convert_m_data, start, end);
            float minima = w_minima_double_seg(convert_m_data, start, end);
            float period = end - start + 1;

            input[n*input_size] = maxima;
            input[n*input_size+1] = minima;
            input[n*input_size+2] = period;
            input[n*input_size+3] = w_mean(convert_m_data + start , period);
            input[n*input_size+4] = w_RMS_seg_double(convert_m_data + start, period);


            input[n*input_size+5] = all_file_data[i].m_1st_feature[0+(j-1)*_MATLAB_OFFSET_FIRST_LEVEL];
            input[n*input_size+6] = all_file_data[i].m_1st_feature[1+(j-1)*_MATLAB_OFFSET_FIRST_LEVEL];
            input[n*input_size+7] = all_file_data[i].m_1st_feature[2+(j-1)*_MATLAB_OFFSET_FIRST_LEVEL];
            input[n*input_size+8] = all_file_data[i].m_1st_feature[3+(j-1)*_MATLAB_OFFSET_FIRST_LEVEL];
            
            mo_types[n] = all_file_data[i].m_type; 
            n++;
        }
    }
    create_cl(input, input_size, n, mo_types, model, output_size, _FALSE, _MASK_LV2, fn);
}

void train_walk_neural_network(TrainingData all_file_data[], int nFiles) {
    float *input;
    float *output;
    unsigned int num_data, num_input, num_output;
    input = output = NULL;
    num_data = 0;
    num_input = _WALK_N_FEATURES;
    num_output = WALK_N_OUTPUTS;

    for(int i = 0; i < nFiles; i++) {
        if ((all_file_data[i].m_type & 0xFF0)==WALK)
            num_data += (all_file_data[i].m_num_divider - 1);
    }

    input = (float *)malloc(sizeof(float)*_WALK_N_FEATURES*num_data);
    output = (float *)malloc(sizeof(float)*WALK_N_OUTPUTS*num_data);
    int n = 0;
    for(int i = 0; i < nFiles; i++){
        if ((all_file_data[i].m_type & 0xFF0)!=WALK) continue;
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

            float a,b,c,d,e,f,g,h;
            input[n*_WALK_N_FEATURES] = maxima;
            a = input[n*_WALK_N_FEATURES+1] = minima;
            b = input[n*_WALK_N_FEATURES+2] = period;
            c = input[n*_WALK_N_FEATURES+3] = w_mean_float(convert_m_data, period);
            d = input[n*_WALK_N_FEATURES+4] = w_RMS_seg(convert_m_data, period);

            input[n*_WALK_N_FEATURES+5] = all_file_data[i].m_1st_feature[0+(j-1)*_MATLAB_OFFSET_FIRST_LEVEL];
            input[n*_WALK_N_FEATURES+6] = all_file_data[i].m_1st_feature[1+(j-1)*_MATLAB_OFFSET_FIRST_LEVEL];
            input[n*_WALK_N_FEATURES+7] = all_file_data[i].m_1st_feature[2+(j-1)*_MATLAB_OFFSET_FIRST_LEVEL];
            input[n*_WALK_N_FEATURES+8] = all_file_data[i].m_1st_feature[3+(j-1)*_MATLAB_OFFSET_FIRST_LEVEL];
            
            e=output[n*WALK_N_OUTPUTS] = (all_file_data[i].m_type == WALK1)?1:-1;
            f=output[n*WALK_N_OUTPUTS+1] = (all_file_data[i].m_type == WALK2)?1:-1;
            g=output[n*WALK_N_OUTPUTS+2] = (all_file_data[i].m_type == WALK3)?1:-1;
            h=output[n*WALK_N_OUTPUTS+3] = (all_file_data[i].m_type == WALK4)?1:-1;
            fprintf(stderr, "%f, %f, %f, %f, %f, %f, %f, %f\n",a,b,c,d,e,f,g,h);
            n++;
        }
    }
    train_from_file_float(input, output, num_data, num_input, num_output, WALK_NEURAL_NETWORK);
}

MoType test_for_motion(MoType motion, double *segment, int length, double* first_level_features)
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


    int n_outputs = get_n_neural_network_outputs(motion);
    double* result = (double*)malloc(sizeof(double)*n_outputs);
    test_from_file_double(features, get_neural_network_name(motion), 1, result);
    int motion_subtype;
    motion_subtype = 0;
    for(int i = 0 ; i < n_outputs; i++) {
        if (result[i] > result[motion_subtype]) {
            motion_subtype = i;
        }
    }
    return(motion + motion_subtype + 1);
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

    if(segmentation(correct_data_buf, size, f, &f_num, div, &div_num, fntype) == _FALSE) 
    {
        *latestMotions = 0;
        *latestMotions_num = 0;
        return;
    };

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
        if(segment_motion[_WALK_RUN_OFFSET] != 0 ||
           segment_motion[_ASC_DSC_OFFSET] != 0) {
            // if it is walk
            double *dp = (double *)malloc(sizeof(double)*(length_of_segment));
            for(int k = 0; k < length_of_segment; k++) {
                dp[k] = correct_data_buf[(start_divider+k) * _DATA_ACQ_SIZE + _ACCEL_X_OFFSET];
            }
 
            if (segment_motion[_WALK_RUN_OFFSET] != 0)
                segment_motion[_WALK_RUN_MOD_OFFSET] = test_for_motion(segment_motion[_WALK_RUN_OFFSET], dp, length_of_segment, &f[_MATLAB_OFFSET_FIRST_LEVEL*i]);

            if (segment_motion[_ASC_DSC_OFFSET] != 0)
                segment_motion[_ASC_DSC_MOD_OFFSET] = test_for_motion(segment_motion[_ASC_DSC_OFFSET], dp, length_of_segment, &f[_MATLAB_OFFSET_FIRST_LEVEL*i]);
        
        }
        memcpy(latestMotions+j*_TOTAL_MOD_COUNT, segment_motion, sizeof(MoType)*_TOTAL_MOD_COUNT);
  }
    prev_num_segments = num_segments > 0 ? num_segments : prev_num_segments;
    *latestMotions_num = num_new_segments;
}

int findinterval(TrainingData file) {
	int i = 0;
	int j;
	j = file.m_divider[i] - file.m_divider[i + 1];
	return j; 
}