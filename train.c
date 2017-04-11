#include "global.h"
#include "matlab_import/rt_nonfinite.h"
#include "matlab_import/get_feature.h"
#include "util.h"
#include "matlab_import/get_feature_terminate.h"
#include "matlab_import/get_feature_emxAPI.h"
#include "matlab_import/get_feature_initialize.h"

#include "FANN/fann_util.h"

#include <stdio.h>
#include <stdlib.h>

int main()
{
    get_feature_initialize();

    int i,j;
    int*   seg_val; 
    double *data_fm  = (double*)malloc(sizeof(double)*_BUFFER*4);
    double *data_val = (double*)malloc(sizeof(double)*_BUFFER*2);
    double *data_buf;
    double* f_m=NULL;
    size_t n, train_num, data_num, seg_num;
    train_num = 0;

    TrainingData td[_SBUFFER];
    i = 0;
    while (i<_TRAIN_DATA_SIZE)
    {
        f_m = (double*)malloc(sizeof(double*)*(_BUFFER));
        seg_val = (int*)malloc(sizeof(int)*_SBUFFER);
        read_from_file(TRAINING_DATASET[i], data_val, &data_num);
        segmentation(data_val, data_num, f_m, &n, seg_val, &seg_num,  fntype[i]);
        memcpy(&data_fm[train_num*5],f_m,sizeof(double)*n*5);
        train_num += n;
        data_buf = (double*)malloc(sizeof(double)*_BUFFER*2);
        for (j = 0; j < data_num; j++)
            memcpy(data_buf+j*7, data_val+j*8+1, sizeof(double)*8);
        td[i].m_data        = data_buf;
        td[i].m_num_data    = data_num;
        td[i].m_divider     = seg_val;
        td[i].m_num_divider = seg_num;
        td[i].m_type        = fntype[i];
        free(f_m);
        i++;
    }
    mo_classfication(data_fm, train_num, TRAINING);
    train_walk_neural_network(td, i);

    get_feature_terminate();
}
