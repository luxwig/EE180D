#define get_index(X,Y,N) X+N*Y

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


void mo_classfication(double* data_fm, size_t n, MoType fntype)
{
    float* input, *output;
    int j, k;
    if (fntype != TEST)
    {
        input  = (float*)malloc(sizeof(float)*n*4);
        output  = (float*)malloc(sizeof(float)*n*3);

        const float output_type[3][3]={{1,-1,-1},{-1,1,-1},{-1,-1,1}};
        for (j = 0; j < n; j++) {
            input[j*4] = data_fm[j*5], 
            input[j*4+1] = data_fm[j*5+1], 
            input[j*4+2] = data_fm[j*5+2], 
            input[j*4+3] = data_fm[j*5+3];
            if (((int)data_fm[j*5+4]&0xF0) != 0xF0)
                memcpy(&output[j*3], output_type[0], sizeof(float)*3);
            else
                memcpy(&output[j*3], output_type[(int)data_fm[j*5+4]&0x00F], sizeof(float)*3);
            for (k = 0; k < 4; k++)
                fprintf(stderr,"%f ", input[j*4+k]);
            fprintf(stderr,"\n");
            for (k = 0; k < 3; k++)
                fprintf(stderr,"%f ", output[j*3+k]);
            fprintf(stderr,"\n");
        }
    
  
        train_from_data(input, output, n, 4, 3, &ann);

    }
    else
    {
    double predict[3];
    for (j = 0; j < n; j++) {
        fprintf(stderr, "%lf %lf %lf %lf\n", 
             data_fm[j*5], 
             data_fm[j*5+1], 
             data_fm[j*5+2], 
             data_fm[j*5+3]);
        test_from_data(&data_fm[j*5], ann, 1, predict);
        //1 parameter: 3 features: [max, min, period, .....
        //2 parameter: 


        fprintf(stderr, "\t%f\t%f\t%f\n",predict[0], predict[1], predict[2]);
        k = predict[0]>predict[1]?0:1;
        k = predict[k]>predict[2]?k:2;
        printf("\t%d\n", k+1);
   }
   }

}
void training_session(const TrainingData* td)
{
    // call Ludwig Training
}
