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

//returns title of neural network associated with   the specific motion 
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

int segmentation(const double* data_buf, const int data_buf_size, double* f, size_t* f_num, int* seg, size_t* seg_num, int fntype) //seg_num is number of dividers, not number of seg
{
	int j, k, n;
	double* data_r = (double*)malloc(sizeof(double)*_BUFFER * 2);
	emxArray_real_T *r;
	emxArray_real_T *pos;
	emxArray_real_T *features;
	emxArray_real_T *m;
	*f_num = 0;

	n = data_buf_size - 1;

	double* gyro_z = (double*)malloc(sizeof(double)*n);
	for (k = 0; k < n; k++) {
		for (j = 1; j < 8; j++)
			data_r[get_index(k, (j - 1), n)] = data_buf[k * 8 + j];
		gyro_z[k] = data_buf[k * 8 + 7];
	}
	double mean = w_mean(gyro_z, n);
	double std = w_std(gyro_z, n);
	double th = mean + std;
	double int_count[_SBUFFER] = {0};  ///what is this?? 
	int count = -1;
	for (j = 0; j < n; j++)
	{
		// below thold
		if ((gyro_z[j]) <= th && gyro_z[j] >= mean - std)
		{
			// first time ignore
			if (count == -1) {
				while ((gyro_z[j]) <= th && gyro_z[j] >= mean - std)
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
	double interval_mean = w_mean(int_count, count);
	double interval_std = w_std(int_count, count);
	double interval_th = interval_mean + 1 * interval_std;
	if (count == 0)
		interval_th = 90;
	fprintf(stderr, "interval_th = %lf\n", interval_th);
	emxInitArray_real_T(&r, 2);
	emxInitArray_real_T(&features, 2);
	emxInitArray_real_T(&pos, 1);

	m = emxCreateWrapper_real_T(data_r, n, 7);

	fprintf(stderr, "MEAN+STD:%f\n", mean + std);
	get_feature(abs(interval_th), mean + std, m, pos, r, features);

	fprintf(stderr, "%d %d\n", r->size[0], r->size[1]);

	if (seg != NULL && seg_num != NULL)
	{
		*seg_num = pos->size[0];
		for (j = 0; j < pos->size[0]; j++)
			seg[j] = (int)(pos->data[j]);
	}
	//iterate through f
	for (j = 0; j < pos->size[0]; j++)
	{
		fprintf(stderr, "%d,", (int)(pos->data[j]));
	}
	fprintf(stderr, "\n");





	//getting y gyro data and extracting features 
	double* y_gyro = (double*)malloc(sizeof(double)*data_buf_size);
	double* abs_max = (double*)malloc(sizeof(double));
	double* rel_min = (double*)malloc(sizeof(double));
	double* rel_max = (double*)malloc(sizeof(double));
	double* ygyro_features = (double*)malloc(sizeof(double)*_FBUFFER);

	get_ygyro(data_buf, data_buf_size, y_gyro); //data_val is complete //data num is size

	/* fprintf(stderr," TESTING \n");
	 for (j = 0; j <data_buf_size; j++)
	{
		fprintf(stderr,"%f,",y_gyro[j]);          //CORRECT
		fprintf(stderr,"\n");
	}
	   fprintf(stderr,"\n");
	*/

	int x;
	int iterate = *seg_num;
	fprintf(stderr, "number of segment dividers: %d \n", iterate);
	for (x = 0; x < iterate - 1; x++) {
		int length = seg[x + 1] - seg[x];
		y_gyro_features_2(y_gyro, length, seg[x], seg[x + 1], abs_max, rel_min, rel_max);
		create_ygyro_feature_array(x, ygyro_features, abs_max, rel_min, rel_max);
	}

	/*  fprintf(stderr," TESTING2 \n");
	 for (j = 0; j < *seg_num *_YGYRO_N_FEATURES - 4; j+=4)
   {
	   fprintf(stderr,"%f, %f, %f, %f",ygyro_features[j], ygyro_features[j+1], ygyro_features[j+2], ygyro_features[j+3]);                          //CORRECT
	   fprintf(stderr,"\n");
   }
   fprintf(stderr,"number of features: %d \n", j); */

	double* z_accel = (double*)malloc(sizeof(double)*data_buf_size);
	double* abs_max_z = (double*)malloc(sizeof(double));
	double* z_accel_at_peak = (double*)malloc(sizeof(double));
	double* abs_min_z = (double*)malloc(sizeof(double));
	double* zaccel_features = (double*)malloc(sizeof(double)*_FBUFFER);
	get_zaccel(data_buf, data_buf_size, z_accel);

	iterate = *seg_num;
	for (x = 0; x < iterate - 1; x++) {
		int length = seg[x + 1] - seg[x];
		z_accel_features_2(z_accel, length, seg[x], seg[x + 1], abs_max_z, z_accel_at_peak);
		*abs_min_z = z_accel_features_1(z_accel, seg[x], seg[x + 1]);
		create_zaccel_feature_array(x, zaccel_features, abs_max_z, z_accel_at_peak, abs_min_z);
	}
	//zaccel features

	double* x_gyro = (double*)malloc(sizeof(double)*data_buf_size);
	double* abs_max_x = (double*)malloc(sizeof(double));
	double* x_gyro_at_peak = (double*)malloc(sizeof(double));
	double* x_gyro_mean = (double*)malloc(sizeof(double));
	double* x_gyro_rms = (double*)malloc(sizeof(double));
	double* x_gyro_kurt = (double*)malloc(sizeof(double));
	double* xgyro_features = (double*)malloc(sizeof(double)*_FBUFFER);
	get_xgyro(data_buf, data_buf_size, x_gyro);

	iterate = *seg_num;
	for (x = 0; x < iterate - 1; x++) {
		int length = seg[x + 1] - seg[x];
		x_gyro_features_2(x_gyro, length, seg[x], seg[x + 1], abs_max_x, x_gyro_at_peak, x_gyro_mean, x_gyro_rms, x_gyro_kurt);
		create_xgyro_feature_array(x, xgyro_features, abs_max_x, x_gyro_at_peak, x_gyro_mean, x_gyro_rms, x_gyro_kurt);
	}   //x_gyro features 

	double* angle_features = (double*)malloc(sizeof(double)*_FBUFFER);
	iterate = *seg_num;
	for (x = 0; x < iterate - 1; x++) {
		int length = seg[x + 1] - seg[x];
		double angle = segment_angle_change(x_gyro, seg[x], seg[x + 1]);
		angle_features[x] = angle; 
	} //angle measurements 
	
	double* x_accel = (double*)malloc(sizeof(double)*data_buf_size);
	double* z_gyro = (double*)malloc(sizeof(double)*data_buf_size);
	double* jump_features = (double*)malloc(sizeof(double)*_FBUFFER);
	double* hang_time = (double*)malloc(sizeof(double));
	get_xaccel(data_buf,data_buf_size, x_accel);
	get_zgyro(data_buf,data_buf_size, z_gyro);
	iterate = *seg_num;
	for (x = 0; x < iterate - 1; x++) {
		x_accel_jump_feature(z_gyro,x_accel,seg[x], seg[x+1],hang_time);
		create_jump_feature_array(x,jump_features,hang_time);
	} //jump measurements 
	

    //adding features to array 
	int seg_iterator = 0;
	if (features->size[0] == 0 || features->size[1] == 0)
		return _FALSE;
	/*
	double matlab_features[4];
	for (j = 0; j < features->size[0]; j++) {
		for (k = 0; k < 4; k++)
			matlab_features[k] = features->data[get_index(j, k, features->size[0])];
	}
	double extracted_feature = matlab_features[2] - matlab_features[0]/(matlab_feature[3]-matlab_feature[1])*/
	FILE* fp; 
	fp = fopen("features.txt","w"); 
    for (j = 0; j < features->size[0]; j++) { //combine these maybe? fp2 - fp1 / t2 - t1?
        for (k = 0; k < 4; k++) 
            f[*f_num*_MATLAB_OFFSET_FIRST_LEVEL+k] = features->data[get_index(j,k,features->size[0])];
		seg_iterator++;
        f[*f_num*_MATLAB_OFFSET_FIRST_LEVEL+4] = ygyro_features[*f_num*_YGYRO_N_FEATURES+4]; //ascend descend feature 
		f[*f_num*_MATLAB_OFFSET_FIRST_LEVEL + 5] = angle_features[*f_num];			//segment angle change 
		f[*f_num*_MATLAB_OFFSET_FIRST_LEVEL + 6] = jump_features[*f_num];	
		f[*f_num*_MATLAB_OFFSET_FIRST_LEVEL+7] = fntype;          //change to 17
		
		
		
        for (k = 0; k < _MATLAB_OFFSET_FIRST_LEVEL; k++)
            fprintf(fp, "\t%lf", f[*f_num*_MATLAB_OFFSET_FIRST_LEVEL+k]);
        fprintf(fp,"\n"); 
        (*f_num)++;
    }
      
    fprintf(fp, "%zu\t%zu\n",*f_num,*seg_num);
    emxDestroyArray_real_T(pos); 
    emxDestroyArray_real_T(features);
    emxDestroyArray_real_T(r);
    emxDestroyArray_real_T(m);

    free(y_gyro);
    free(abs_max);
    free(rel_min);
    free(rel_max);
    free(ygyro_features);
		
	fclose(fp); 
    return (*f_num+1==*seg_num);

}

MoType test_cl(double* features, const MoType* mo_status, int mo_status_num, const char* filename)
{
    int i, max;
    double* predict = (double*)malloc(sizeof(double)*(mo_status_num + 1));
    test_from_file_double(features, filename, 1, predict);
    max = 0;
    for (i = 1; i < mo_status_num; i++)
        if (predict[i] > predict[max])
            max = i;
    if (max < mo_status_num)
        return mo_status[max];
    else 
        return NONE;
}

void create_cl(double* features, int features_num, int seg_num, MoType* mo_types, const MoType* mo_status, int mo_status_num, int mask, const char* filename)
{
    // create output array
    int i, j;
    double* output = (double*)malloc(sizeof(double)*(seg_num*(mo_status_num+1)));
    short flag;
    for (i = 0; i < seg_num; i++)
    {
        flag = _FALSE;
        for (j = 0; j < mo_status_num; j++)
            if (
               (output[i*(mo_status_num)+j] = (mo_status[j]==(mo_types[i]&mask))?1:-1) == 1)
                flag = _TRUE;
        if (!flag && mo_status[mo_status_num-1]==_NONE)
            output[i*(mo_status_num)+mo_status_num-1] = 1;
        for (j = 0; j < features_num; j++)
            fprintf(stderr, "%lf\t", features[i*features_num+j]);
        fprintf(stderr, "\n");
        for (j = 0; j < mo_status_num; j++)
            fprintf(stderr,"%lf\t", output[i*(mo_status_num)+j]);
        fprintf(stderr, "\n");
    }
    train_from_file_double(features, output, seg_num, features_num, mo_status_num, filename);
    free(output);
}


void mo_training(double* data_fm, size_t n)
{
    double *features = (double*)malloc(sizeof(double)*n*_FIRST_LEVEL_FEATURES);
    MoType* mo_types = (MoType*)malloc(sizeof(MoType)*n);  
    int i;
   /*  for( int x = 0; x < n*_FIRST_LEVEL_FEATURES; x++)
          fprintf(stderr, " %f \n" , data_fm[x]);
        exit(1); 
        */
    //preprocess features and output
    for (i = 0; i < n; i++)
    {
        memcpy(features+_FIRST_LEVEL_FEATURES*i, 
                data_fm+_MATLAB_OFFSET_FIRST_LEVEL*i, 
                sizeof(double)*_FIRST_LEVEL_FEATURES);
        mo_types[i] = (int)data_fm[i*_MATLAB_OFFSET_FIRST_LEVEL+_MATLAB_OFFSET_FIRST_LEVEL-1]; //not sure if mo_type will be right
    }
	/*feature analysis .txt 
	File *feat_file; 
	feat_file = fopen("feature_analysis.txt", "w"); 
	MoType previous = mo_types[0];
	MoType current = mo_types[0];
	for(int i = 0; i < n; i++){
		current = mo_types[i]; 
		if(curret != previous)
				
		previous = current; 	
	}
    fclose(feat_file); 
	*/
        
        
    // train ASC_DSC
    create_cl(features, _FIRST_LEVEL_FEATURES, n, mo_types, ASC_DSC_MODEL, _ASC_DSC_SIZE, _MASK_LV1, ASC_DSC_FN);
    // train WALK
    create_cl(features, _FIRST_LEVEL_FEATURES, n, mo_types, WALK_RUN_MODEL, _WALK_RUN_SIZE, _MASK_LV1, WALK_RUN_FN); 
    // train TL_TR
    create_cl(features, _FIRST_LEVEL_FEATURES, n, mo_types, TL_TR_MODEL, _TL_TR_SIZE, _MASK_LV1, TL_TR_FN);   //++
    //train JUMP
    create_cl(features, _FIRST_LEVEL_FEATURES, n, mo_types, JUMP_MODEL, _JMP_SIZE, _MASK_LV1, JUMP_FN);  //we aren't doing jump yet  
    // train FIRST_LV_ALL
    create_cl(features, _FIRST_LEVEL_FEATURES, n, mo_types, FIRST_LV_ALL_MODEL, _1ST_LV_ALL_SIZE,  _MASK_LV1, FIRST_LV_ALL_FN);
    
}


void mo_classfication(double* data_fm, size_t n, MoType* result)
{
    int flag = _FALSE;
    flag |= 
        ( result[_ASC_DSC_OFFSET] = test_cl(data_fm, ASC_DSC_MODEL, _ASC_DSC_SIZE, ASC_DSC_FN));
    
    flag |= 
        ( result[_WALK_RUN_OFFSET] = test_cl(data_fm, WALK_RUN_MODEL, _WALK_RUN_SIZE, WALK_RUN_FN));
         
    flag |=
        ( result[_TL_TR_OFFSET] = test_cl(data_fm, TL_TR_MODEL, _TL_TR_SIZE, TL_TR_FN));

    result[_JMP_OFFSET] = 0;    
    if (!flag) {
        result[_1ST_LV_ALL_OFFSET] =
            test_cl(data_fm, FIRST_LV_ALL_MODEL, _1ST_LV_ALL_SIZE,
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
    create_cl(input, input_size, n, mo_types, model, output_size, _MASK_LV2, fn);
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
        // int second_end_divider = div[i+2];

        int length_of_segment = end_divider - start_divider + 1;
        // int length_of_two_segments = second_end_divider - start_divider + 1;

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

//////////////////////////////xaccel_ygyro functions ///////////////////////

void get_ygyro(const double* data_val, const int data_buf_size, double *y_gyro) //data_val is complete //data num is size
{
	int j;
	for (j = 0; j < data_buf_size; j++)                     //might need to be data_buf_size -1
   y_gyro[j] = data_val[j*_DATA_ACQ_SIZE + _GYRO_Y_OFFSET];
}


//individual will need to stand still to eliminate offset of gravity
//double or float, whatever it is
double get_gravity_offset( double x_accel_data[], int iterations) {
int total = 0;
int x = 0;
while( x < iterations)
{
	total += x_accel_data[x];
	x++;
}
return total/x; 
}

// takes an xaccel segment and eliminates offset
void eliminate_offset( double *x_accel_data, int segment_length, double gravity_offset ) {
	int i; 
	for (i =0; i < segment_length; i++){
		x_accel_data[i] += gravity_offset;  //should be -= if using real time and not macro 
	}
}
double y_gyro_features_1( const double* segment, int begin, int end){
	return w_minima_double_seg((double*)segment, begin, end);
}

void y_gyro_features_2( const double* segment, int segment_length, int begin, int end, double* abs_max, double* rel_min, double* rel_max)
{
	//find absolute max  of first 3/4
    int c, index;
    double max;
    max = segment[begin];
    index = begin;
    for (c = index; c < (end - (segment_length/2)); c++) {
        if (segment[c] > max) {
            index = c;
            max = segment[c];
        }
    }
	*abs_max = segment[index]; 
	
	//start at least 50 points away for next maximum, could be 25 or 50 
	//or find a better way to do this //what about when 0 is crossed ?
	int index_2 = index + 45; 
	max = 0;
	for(c = index_2; c < end; c++) {
		if(segment[c] > max) {
			index_2 = c; 
			max = segment[c]; 
		}
	}
	*rel_max = segment[index_2];
	
	//find minimum between them 
	*rel_min = w_minima_double_seg((double*)segment, index, index_2);
}

void create_ygyro_feature_array(int i, double* ygyro_features, double* abs_max, double* rel_min, double* rel_max)
{
   double feature_1 = *abs_max / *rel_max;               /* max1/max2 */
   double feature_2 = *abs_max / *rel_min ;               /* max1/min */
   double feature_3 = *rel_max / *rel_min ;                /*max2/min */
   double feature_4 = (*rel_max + *abs_max) / *rel_min;    /* max1 + max2/min*/
   double feature_5 = sqrt((pow((*abs_max - *rel_min), 2) + pow((*rel_max - *rel_min), 2)) / 2);  /*new composite feature*/

	ygyro_features[i*_YGYRO_N_FEATURES] = feature_1; 
    ygyro_features[i*_YGYRO_N_FEATURES +1] = feature_2;                                                            
    ygyro_features[i*_YGYRO_N_FEATURES +2] =  feature_3;                                                            
    ygyro_features[i*_YGYRO_N_FEATURES +3] =  feature_4;                                                          
	ygyro_features[i*_YGYRO_N_FEATURES + 4] = feature_5; 
}

void get_zaccel(const double* data_val, const int data_buf_size, double *z_accel) //data_val is complete //data num is size
{
	int j;
	for (j = 0; j < data_buf_size; j++)                     //might need to be data_buf_size -1
    z_accel[j] = data_val[j*_DATA_ACQ_SIZE + _ACCEL_Z_OFFSET];
}
void z_accel_features_2(const double* segment, int segment_length, int begin, int end, double* abs_max, double* z_accel_at_peak)
{
    int c, index;
    double max;
    max = segment[begin];
    index = begin;
    for (c = index; c < end; c++) {
        if (segment[c] > max) {
            index = c;
            max = segment[c];
        }
    }
	*abs_max = segment[index]; 
	*z_accel_at_peak = segment[begin]; 
}

double z_accel_features_1( const double* segment, int begin, int end){
	return w_minima_double_seg((double*)segment, begin, end);
}

double x_gyro_features_1( const double* segment, int begin, int end){
  return w_minima_double_seg((double*)segment, begin, end);
}

void create_zaccel_feature_array(int i, double* zaccel_features, double* abs_max, double* z_accel_at_peak, double* abs_min)
{
   double feature_1 = *z_accel_at_peak;               /* peak z gyro value at z accel min */
   double feature_2 = *abs_min;               		  /* accel z minimum value*/
   double feature_3 = *abs_max - *abs_min;            /*accel z min max difference*/
   
   
   zaccel_features[i*_ZACCEL_N_FEATURES] = feature_1; 
   zaccel_features[i*_ZACCEL_N_FEATURES +1] = feature_2;                                                            
   zaccel_features[i*_ZACCEL_N_FEATURES +2] =  feature_3;                                                            
                                                          
               
}


///// X gyro features/////
void get_xgyro(const double* data_val, const int data_buf_size, double *x_gyro) //data_val is complete //data num is size
{
	int j;
	for (j = 0; j < data_buf_size; j++)                     //might need to be data_buf_size -1
    x_gyro[j] = data_val[j*_DATA_ACQ_SIZE + _GYRO_X_OFFSET];
}
void x_gyro_features_2(const double* segment, int segment_length, int begin, int end, double* abs_max, double* x_gyro_at_peak, double* x_gyro_mean, 
double* x_gyro_rms, 
double* x_gyro_kurt)
{
    int c, index;
    double max;
    max = segment[begin];
    index = begin;
    for (c = index; c < end; c++) {
        if (segment[c] > max) {
            index = c;
            max = segment[c];
        }
    }
	*abs_max = segment[index]; 
	*x_gyro_at_peak = segment[begin]; 
	*x_gyro_mean = w_mean(segment, segment_length); 
	*x_gyro_rms = w_RMS_seg_double (segment, segment_length);
	*x_gyro_kurt = calculate_kurtosis_d(segment, segment_length); 
}

void create_xgyro_feature_array(int i, double* xgyro_features, double* abs_max, double* x_gyro_at_peak, double* x_gyro_mean, double* x_gyro_rms, double* x_gyro_kurt)
{
   double feature_1 = *x_gyro_at_peak;               /* peak z gyro value at x gyro peak */  //maybe should be relative to surrounding data 
   double feature_2 = *x_gyro_mean;           		  /* x gyro mean*/
   double feature_3 = *x_gyro_rms;          
   double feature_4 = *x_gyro_kurt;
   double feature_5 = *abs_max;
   
   xgyro_features[i*_XGYRO_N_FEATURES] = feature_1; 
   xgyro_features[i*_XGYRO_N_FEATURES +1] = feature_2;                                                            
   xgyro_features[i*_XGYRO_N_FEATURES +2] =  feature_3; 
   xgyro_features[i*_XGYRO_N_FEATURES+3] = feature_4; 
   xgyro_features[i*_XGYRO_N_FEATURES+4] = feature_5;    
                                                                       
}

double segment_angle_change(double* lpf_input, int begin, int end) {
	double mdeg; // mdps/digit gyro setting 
	switch (a_rate) {
	case 245:
		mdeg = 8.75;
		break; 
	case 500:
		mdeg = 17.50;
		break;
	case 2000:
		mdeg = 70.0; 
		break; 
	default: 
		mdeg = 17.50;  //i think this is default anyway 
		break;
	}
	double deg_d = mdeg;  // degrees per digit 
	double angle = 0;  //assume angle 0 start as we are looking for angle change in a given segment anyway 
	double gyroRate;
	for (int i = begin; i < end; i++) {
		gyroRate = deg_d*lpf_input[i]*dx; 
		angle += gyroRate;
	}
	return angle; 

}

/*
Angular rate FS = ±245 dps 8.75 mdps/digit

Angular rate FS = ±500 dps 17.50 mdps/digit
Angular rate FS = ±2000 dps 70 mdps/digit
*/

void get_xaccel(const double* data_val, const int data_buf_size, double *x_accel) //data_val is complete //data num is size
{
	int j;
	for (j = 0; j < data_buf_size; j++)                     //might need to be data_buf_size -1
		x_accel[j] = data_val[j*_DATA_ACQ_SIZE + _ACCEL_X_OFFSET];
}
void get_zgyro(const double* data_val, const int data_buf_size, double *z_gyro)
{
	int j;
	for (j = 0; j < data_buf_size; j++)                     //might need to be data_buf_size -1
		z_gyro[j] = data_val[j*_DATA_ACQ_SIZE + _GYRO_Z_OFFSET];
}

void x_accel_jump_feature(const double* z_gyro, const double* x_accel, int begin, int end, double* hang_time)
{
    int c, index_max, index_min;
    double z_max, z_min;
    index_max = begin; //assume max is start of strde 
	index_min = begin; 
	for( c = index_min; c < end; c++){ //find min value 
		if(z_gyro[c] < z_min){
			index_min = c;
		z_min = z_gyro[c]; }
	}
	
	//if the min is before the max, we are definitely not looking at a jump; 
	if(index_min < index_max){
		*hang_time = -1.0; //error check 
	}
	int n = (index_min - (index_max + 1)); 
	double sum = 0; 
	for (c = index_max + 1; c < index_min; c++){
		sum += pow((x_accel[c]),2);
	}
	sum /= ((double)n-1.0); 
	*hang_time = sqrt(sum);
}

/*
void ascend_descend_feature(const double *z_gyro, const double* x_accel, int begin int end, double * peak_character, double threshold)
{
	double a, b; 
	for(c = begin; c < end; c++){ //count the troughs in a given signal  
		if(){
			index_min = c;
		z_min = z_gyro[c]; }
	}
	
}
*/ 

void create_jump_feature_array(int i, double* jump_features, double* hang_time)
{
   double feature_1 = *hang_time;              
   jump_features[i] = feature_1;                                                                        
}


void low_pass_filter(double* input, double* output, int filter_order, float f_cutoff){

float f0 = 0.0f; //ignored
float As = 0.0f;      //ignored
float Ap = 0.0f;	//ignored
iirfilt_rrrf lpf = iirfilt_rrrf_create_prototype(LIQUID_IIRDES_BUTTER,LIQUID_IIRDES_LOWPASS,LIQUID_IIRDES_SOS,filter_order,f_cutoff,f0,Ap,As);
iirfilt_rrrf_execute(lpf,(float)input,(float)*output);
iirfilt_rrrf_destroy(lpf);
}


