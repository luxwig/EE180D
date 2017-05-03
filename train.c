#include "global.h"
#include "matlab_import/rt_nonfinite.h"
#include "matlab_import/get_feature.h"
#include "util.h"
#include "matlab_import/get_feature_terminate.h"
#include "matlab_import/get_feature_emxAPI.h"
#include "matlab_import/get_feature_initialize.h"
#include "matlab_import/interp_initialize.h"
#include "matlab_import/interp_terminate.h"
#include "matlab_import/interp.h"

#include "FANN/fann_util.h"

#include <stdio.h>
#include <stdlib.h>

int main()
{
    get_feature_initialize();
    interp_initialize();
    int i,j;
    int*   seg_val; 
    double *data_fm  = (double*)malloc(sizeof(double)*_BUFFER*4);
    double *data_val = (double*)malloc(sizeof(double)*_BUFFER*2);
    double *data_buf;
    double *r        = (double*)malloc(sizeof(double)*_BUFFER*2);
    double* f_m=NULL;
    size_t n, train_num, data_num, seg_num;
    train_num = 0;

    double *all_r = (double*)malloc(sizeof(double)*_RESAMPLE_SIZE*_SBUFFER);
    TrainingData td[_SBUFFER];
    i = 0;
    double *resample = (double*)malloc(sizeof(double)*_RESAMPLE_SIZE*6*_BUFFER); 
    while (i<_TRAIN_DATA_SIZE)
    {
        fprintf(stderr, "%s\n", TRAINING_DATASET[i]);
        f_m = (double*)malloc(sizeof(double*)*(_BUFFER));
        seg_val = (int*)malloc(sizeof(int)*_SBUFFER);
        read_from_file(TRAINING_DATASET[i], data_val, &data_num);
        segmentation(data_val, data_num, f_m, &n, seg_val, &seg_num,  fntype[i],r);
        resample_data(data_val, seg_val, seg_num, resample+_RESAMPLE_SIZE*6*train_num); 
        memcpy(&data_fm[train_num*_MATLAB_OFFSET_FIRST_LEVEL],f_m,sizeof(double)*n*_MATLAB_OFFSET_FIRST_LEVEL);
        memcpy(all_r+_RESAMPLE_SIZE*train_num,r, sizeof(double)*n*_RESAMPLE_SIZE);
        train_num += n;
        data_buf = (double*)malloc(sizeof(double)*_BUFFER*2);
        for (j = 0; j < data_num; j++)
            memcpy(data_buf+j*7, data_val+j*8+1, sizeof(double)*8);
        td[i].m_data        = data_buf;
        td[i].m_num_data    = data_num;
        td[i].m_divider     = seg_val;
        td[i].m_num_divider = seg_num;
        td[i].m_type        = fntype[i];
        td[i].m_1st_feature = f_m;
        i++;
    }
    int k;
    for ( i = 0; i < train_num; i++)
    {
        fprintf(stderr,"seg %d\n",i);
        for (j = 0; j <6; j++) {
            fprintf(stderr,"axis %d-%d\n",i,j);
            for (k = 0; k < 200; k++)
                fprintf(stderr,"%lf\n",resample[i*1200+j*200+k]);
        }
    }
    mo_training(resample, data_fm, train_num);
    train_lv2_neural_network(td, _TRAIN_DATA_SIZE, WALK, _WALK_N_FEATURES, _WALK_LV2_SIZE, WALK_LV2_MODEL, WALK_NEURAL_NETWORK);
    train_lv2_neural_network(td, _TRAIN_DATA_SIZE, RUN, _RUN_N_FEATURES, _RUN_LV2_SIZE, RUN_LV2_MODEL, RUN_LV2_FN);
    train_lv2_neural_network(td, _TRAIN_DATA_SIZE, ASC, _ASCEND_N_FEATURES, _ASC_LV2_SIZE, ASC_LV2_MODEL, ASC_LV2_FN); 
    train_lv2_neural_network(td, _TRAIN_DATA_SIZE, DSC, _DESCEND_N_FEATURES, _DSC_LV2_SIZE, DSC_LV2_MODEL, DSC_LV2_FN);
    get_feature_terminate();
    interp_terminate();
}
