#define get_index(X,Y,N) X+N*Y
#define WALK_N_FEATURES 5
#define ASC_N_FEATURES 5
#define DSC_N_FEATURES 5

#define WALK_N_OUTPUTS 4
#define ASC_N_OUTPUTS 2
#define DSC_N_OUTPUTS 2

#define WALK_MAXIMA_INDEX 0
#define WALK_MINIMA_INDEX 1
#define WALK_PERIOD_INDEX 2
#define _GNU_SOURCE
#define _FILENUM 26 //this is gonna change 
	
#include "util.h"
#include "matlab_import/rt_nonfinite.h"
#include "matlab_import/get_feature.h"
#include "matlab_import/get_feature_terminate.h"
#include "matlab_import/get_feature_emxAPI.h"
#include "matlab_import/get_feature_initialize.h"
#include "FANN/fann_util.h"

#include <stdio.h>
#include <stdlib.h>


/*
    fn: filename
    f: actual data
    f_num: number of deatures
    seg: indexes of dividers
    segm_num: number of segments
    fntype: filename type
*/

static struct fann* ann;
void segmentation(const char* fn, double* f, int* f_num, int* seg, int* seg_num, int fntype, double* data_val, int* data_len)
{
    FILE *fd;
    int j, k;
    double data[_BUFFER*2], data_r[_BUFFER*2];
    char* line = NULL;
    double temp;
    size_t read, len, n;
 
    fd = fopen(fn, "r"); 
    getline(&line, &len, fd);
    getline(&line, &len, fd);

    emxArray_real_T *r;
    emxArray_real_T *pos;
    emxArray_real_T *features;
    emxArray_real_T *m;
    *f_num = 0;

    n = 0;
    while ((read = getline(&line, &len, fd))!=-1){
        sscanf(line,"%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
               &temp, &data[n*7], &data[n*7+1], &data[n*7+2],
               &data[n*7+3], &data[n*7+4], &data[n*7+5],&data[n*7+6]);
        n++;
    }

    fclose(fd);
    n--;
    for (k = 0; k < n; k++)
        for (j = 0 ; j<7; j++)
            data_r[get_index(k,j,n)] = data[k*7+j];
            
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

    for (j = 0; j < n*7; j++)
        data_val[j] = data[j];
    *data_len = n;
}


MoType mo_classfication(double* data_fm, size_t n, MoType fntype)
{
    double* input, *output;
    int j, k;
    if (fntype == TRAINING) 
    {
        input  = (double*)malloc(sizeof(double)*n*5);			//added input for interval 
        output  = (double*)malloc(sizeof(double)*n*4);			//added output for run 

		const double output_type[4][4] = { {1,-1,-1,-1},{-1,1,-1,-1},{-1,-1,1,-1},{-1,-1,-1,1} };	//added -1,-1,-1,1 output for run 
        for (j = 0; j < n; j++) {
            memcpy(input+5*j,data_fm+6*j, sizeof(double)*5);	//changed to copy 5 input features 	
			switch ((int)data_fm[j * 6 + 5]) {
			case 1:
			case 2:
			case 3:
			case 4:
				memcpy(&output[j * 4], output_type[0], sizeof(double) * 4);
        break;
			case 241:
      case 243:
      case 244:
				memcpy(&output[j * 4], output_type[1], sizeof(double) * 4);
        break;
			case 242:
      case 245:
      case 246: 
				memcpy(&output[j * 4], output_type[2], sizeof(double) * 4);
        break;
			case 5:
				memcpy(&output[j * 4], output_type[3], sizeof(double) * 4);
        break;
			default: 
				fprintf(stderr, "data_fm passed wrong type %d \n", (int)data_fm[j*6+5]);
				//exit(1);
        //return TRAINING; 
        break; 
			}
			for (k = 0; k < 5; k++)
                fprintf(stderr,"%f ", input[j*5+k]);
            fprintf(stderr,"\n");
            for (k = 0; k < 4; k++)
                fprintf(stderr,"%f ", output[j*4+k]);
            fprintf(stderr,"\n");
        }
    
  
        train_from_data_double(input, output, n, 5, 4, &ann);		//changed
        return TRAINING;
    }
    else
    {
        double predict[4];										//changed to 4
        test_from_data_double(data_fm, ann, 1, predict);
        //1 parameter: 3 features: [max, min, period, .....
        //2 parameter: 


        fprintf(stderr, "\t%f\t%f\t%f\n",predict[0], predict[1], predict[2]);
		if (predict[0] > predict[1] && predict[0] > predict[2] && predict[0] > predict[3])
			return TRAINING;						// is this the right return for walk???
		else if (predict[1] > predict[0] && predict[1] > predict[2] && predict[1] > predict[3])
			return ASC;
		else if (predict[2] > predict[0] && predict[2] > predict[1] && predict[2] > predict[3])
			return DSC;
		else if (predict[3] > predict[0] && predict[3] > predict[1] && predict[3] > predict[2])
			return RUN; 
		else
		{
			fprintf(stderr, "\t%f %f %f %f\n", predict[0], predict[1], predict[2], predict[3]);
			exit(1);
		}
    }    
}


static struct fann* walk_neural_network;
static struct fann* asc_neural_network; 
static struct fann* dsc_neural_network; 

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
            output[n*WALK_N_OUTPUTS] = (all_file_data[i].m_type == WALK1)*2-1;
            output[n*WALK_N_OUTPUTS+1] = (all_file_data[i].m_type == WALK2)*2-1;
            output[n*WALK_N_OUTPUTS+2] = (all_file_data[i].m_type == WALK3)*2-1;
            output[n*WALK_N_OUTPUTS+3] = (all_file_data[i].m_type == WALK4)*2-1;
            n++;
        }
    }
    train_from_data_float(input, output, num_data, num_input, num_output, &walk_neural_network);
}


void train_asc_neural_network(TrainingData all_file_data[], int nFiles) {
    float *input;
    float *output;
    unsigned int num_data, num_input, num_output;
    input = output = NULL;
    num_data = 0;
    num_input = ASC_N_FEATURES;
    num_output = ASC_N_OUTPUTS;

    for(int i = 0; i < nFiles; i++) {
        if (all_file_data[i].m_type == ASC1 || all_file_data[i].m_type == ASC2)
          num_data += (all_file_data[i].m_num_divider - 1);
    }

    input = (float *)malloc(sizeof(float)*ASC_N_FEATURES*num_data);
    output = (float *)malloc(sizeof(float)*ASC_N_OUTPUTS*num_data);
    int n = 0;
    for(int i = 0; i < nFiles; i++){
        if (all_file_data[i].m_type != ASC1 || all_file_data[i].m_type != ASC2) continue;
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
            input[n*ASC_N_FEATURES] = maxima;
            input[n*ASC_N_FEATURES+1] = minima;
            input[n*ASC_N_FEATURES+2] = period;
            input[n*ASC_N_FEATURES+3] = w_mean_float(convert_m_data, period);
            input[n*ASC_N_FEATURES+4] = w_RMS_seg(convert_m_data, period);
            output[n*ASC_N_OUTPUTS] = (all_file_data[i].m_type == ASC1)*2-1;                //ASC 1 is [1,-1] ASC 2 is [-1,1]
            output[n*ASC_N_OUTPUTS+1] = (all_file_data[i].m_type == ASC2)*2-1;
            n++;
        }
    }
    train_from_data_float(input, output, num_data, num_input, num_output, &asc_neural_network);
}


void train_dsc_neural_network(TrainingData all_file_data[], int nFiles) {
    float *input;
    float *output;
    unsigned int num_data, num_input, num_output;
    input = output = NULL;
    num_data = 0;
    num_input = DSC_N_FEATURES;
    num_output = DSC_N_OUTPUTS;

    for(int i = 0; i < nFiles; i++) {
        if (all_file_data[i].m_type == DSC1 || all_file_data[i].m_type == DSC2) 
        num_data += (all_file_data[i].m_num_divider - 1);
    }

    input = (float *)malloc(sizeof(float)*WALK_N_FEATURES*num_data);
    output = (float *)malloc(sizeof(float)*WALK_N_OUTPUTS*num_data);
    int n = 0;
    for(int i = 0; i < nFiles; i++){
        if (all_file_data[i].m_type != DSC1 || all_file_data[i].m_type != DSC2) continue;
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
            input[n*DSC_N_FEATURES] = maxima;
            input[n*DSC_N_FEATURES+1] = minima;
            input[n*DSC_N_FEATURES+2] = period;
            input[n*DSC_N_FEATURES+3] = w_mean_float(convert_m_data, period);
            input[n*DSC_N_FEATURES+4] = w_RMS_seg(convert_m_data, period);
            output[n*DSC_N_OUTPUTS] = (all_file_data[i].m_type == DSC1)*2-1;
            output[n*DSC_N_OUTPUTS+1] = (all_file_data[i].m_type == DSC2)*2-1; //ASC 1 is [1,-1] ASC 2 is [-1,1]
            n++;
        }
    }
    train_from_data_float(input, output, num_data, num_input, num_output, &dsc_neural_network);
}



MoType test_for_walking_speed(double *segment,int length) 
{
    double maxima = w_maxima_double_seg(segment, 0, length);
    double minima = w_minima_double_seg(segment, 0, length);
    double period = (double)length;
    double mean = (double)w_mean(segment,length);
    double RMS = w_RMS_seg_double(segment,length);
    double features[] = {maxima, minima, period, mean, RMS};
    double result[4];
    test_from_data_double(features, walk_neural_network, 1, result);
    int maximum = 0;
    for(int i = 0 ; i < WALK_N_OUTPUTS; i++) {
        if (result[i] > result[maximum]) {
            maximum = i;
        }
    }
    return (maximum+1);
}


MoType test_for_asc_speed(double *segment,int length) 
{
    double maxima = w_maxima_double_seg(segment, 0, length);
    double minima = w_minima_double_seg(segment, 0, length);
    double period = (double)length;
    double mean = (double)w_mean(segment,length);
    double RMS = w_RMS_seg_double(segment,length);
    double features[] = {maxima, minima, period, mean, RMS};
    double result[2];
    test_from_data_double(features, asc_neural_network, 1, result);
    if(result[0] > result[1])
      return ASC1;
    else
      return ASC2;
}


MoType test_for_dsc_speed(double *segment,int length) 
{
    double maxima = w_maxima_double_seg(segment, 0, length);
    double minima = w_minima_double_seg(segment, 0, length);
    double period = (double)length;
    double mean = (double)w_mean(segment,length);
    double RMS = w_RMS_seg_double(segment,length);
    double features[] = {maxima, minima, period, mean, RMS};
    double result[2];
    test_from_data_double(features, dsc_neural_network, 1, result);
   if(result[0] > result[1])
     return DSC1; 
   else
     return DSC2; 
}








int findinterval(TrainingData file) {
	int i = 0;
	int j;
	j = file.m_divider[i] - file.m_divider[i + 1];
	return j; 
}

void insert_interval_training(size_t train_num, double* data_fm, TrainingData td[], const MoType fntype[], float sd_each_segment[] ) //train num is the number of segments..
{
	//shift in the right direction data fm to make space for interval 
	int total = train_num * 5; 
	int c, d;
	for (c = total - 1; c > 3; c -= 5) //at end of each segment 
	{
		for (d = total; d > c; d--)
		{
			data_fm[d] = data_fm[d - 1];
		}
		total += 1;
	}
 
	/*//fill with correct value of segment 
	int intervals[2000]; //provide sufficient room for excess data sets later 
	int position = 0;
	int i;
	for  (i = 0; i < _FILENUM && fntype[i] != TEST; i++) //fill array with intervals 
	{
		for (int j = 0; j < td[i].m_num_divider; j++)
		{
			intervals[position + j] = td[i].m_divider[j + 1] - td[i].m_divider[j];
		}
		position += (td[i].m_num_divider -1);
	}
 */
 
	int x = 0;
	//insert interval to array of features
	for (int i = 4; i < total; i += 6)
	{
		data_fm[i] = (double)sd_each_segment[x];
		x++;
	}
}

void insert_interval_testing(size_t n, double* f_m, float sd_each_segment[]) {
		//code for inserting interval 
			//shift in the right direction data fm to make space for interval 
		int total = n * 5;
		int c, d; 
		for (c = total - 1; c > 3; c -= 5) //at end of each segment 
			 {
			for (d = total; d > c; d--)
				 {
					f_m[d] = f_m[d - 1];
				}
			total += 1;
			}
			/*	//fill with correct value of segment intervals
			int intervals[1000];										//sufficient size for test cases 
		for (int j = 0; j < n; j++)											//if there are 2 segments there are 3 dividers right?
			 {
			intervals[j] = seg_val[j + 1] - seg_val[j];
			}
		*/
   
			int x = 0; 		//insert interval to array of features
			for (int i = 4; i < total; i += 6)
			 {
				f_m[i] = (double)sd_each_segment[x];
        x++;
			}
				//end code for interval insertion 
			return;
			}
		
	

