#define get_index(X,Y,N) X+N*Y
#define WALK_N_FEATURES 3
#define WALK_N_OUTPUTS 4
#define WALK_MAXIMA_INDEX 0
#define WALK_MINIMA_INDEX 1
#define WALK_PERIOD_INDEX 2

#include "util.h"
#include "matlab_import/rt_nonfinite.h"
#include "matlab_import/get_feature.h"
#include "matlab_import/get_feature_terminate.h"
#include "matlab_import/get_feature_emxAPI.h"
#include "matlab_import/get_feature_initialize.h"


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

void segmentation(const char* fn, double* f, int* f_num, double* seg, int* seg_num, int fntype)
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
        memcpy(seg, pos->data, sizeof(double)*(pos->size[0]));
    }

    emxDestroyArray_real_T(pos); 
    emxDestroyArray_real_T(features);
    emxDestroyArray_real_T(r);
    emxDestroyArray_real_T(m);
}


void training_session(const TrainingData* td)
{
    // call Ludwig Training
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Training walk
////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 stored_features_contig: features of each segment stored contiguously
 nth_seg: the nth segment of data to extract a set of features from
 begin: begin index in x_axis accel
 end: end index in x_axis accel
 x_axis: raw x_axis accel data
 */
void fetch_features_in_segment(float* stored_features_contig, int nth_seg, int begin, int end, float* x_axis) {
    stored_features_contig[nth_seg*WALK_N_FEATURES + WALK_MAXIMA_INDEX] = w_maxima_seg(x_axis, begin, end);
    stored_features_contig[nth_seg*WALK_N_FEATURES + WALK_MINIMA_INDEX] = w_minima_seg(x_axis, begin, end);
    stored_features_contig[nth_seg*WALK_N_FEATURES + WALK_PERIOD_INDEX] = (begin - end);
    return;
}


void compile_features(TrainingData* data_from_file, float* stored_features_contig) {
    int number_of_dividers = data_from_file->m_num_divider;
    int number_of_segments = number_of_dividers - 1;
    stored_features_contig = (float *)malloc(number_of_segments*WALK_N_FEATURES);
    int begin, end;
    for(int i = 0; i < number_of_segments; i++) {
        begin = data_from_file->m_divider[i];
        end = data_from_file->m_divider[i+1];
        fetch_features_in_segment(stored_features_contig, i, begin, end, (float *)data_from_file->m_data);
    }
}

// void train_from_data(
//         fann_type*   input, //contiguous features one array
//         fann_type*   output, //what each contiguous feature should output
//         const unsigned int num_data, //how many intervals
//         const unsigned int num_input, //number of features
//         const unsigned int num_output, //number of options
//         struct fann** ann) //the neural network object
// {
// }
// struct TrainingData_struct
// {
//     double* m_data;
//     int     m_num_data;
//     int*    m_divider;
//     int     m_num_divider;
//     MoType m_type;
// };

static struct fann* walk_neural_network;

void train_walk_neural_network(TrainingData* all_file_data[], int nFiles) {
    float *input;
    float *output;
    float *single_file_stored_features_contig;
    input = output = single_file_stored_features_contig = NULL;

    int total_nSamples = 0;
    int total_nSegments = 0;
    for(int i = 0; i < nFiles; i++) {
        total_nSamples += all_file_data[i]->m_num_data;
        total_nSegments += (all_file_data[i]->m_num_divider - 1);
    }

    input = (float*)malloc(total_nSamples*sizeof(float));
    output = (float*)malloc(total_nSegments*sizeof(float));

    int copied_segments = 0;
    for(int i = 0; i < nFiles; i++) {
        int nSegments = all_file_data[i]->m_num_data- 1;
        for(int j = 0; j < nSegments; j++) {
            output[j+copied_segments] = (float)all_file_data[i]->m_type;
        }
        compile_features(all_file_data[i], single_file_stored_features_contig);
        /*
         TODO: use stored_features_contig to train neural network
        */
        memcpy(input + WALK_N_FEATURES*copied_segments, single_file_stored_features_contig, sizeof(float));
        copied_segments += (all_file_data[i]->m_num_divider - 1);
        free(single_file_stored_features_contig);
    }
    train_from_data(input, output, total_nSegments, WALK_N_FEATURES, WALK_N_OUTPUTS, &walk_neural_network);
}

void test_walk_data(float*data, int n) {
    float maxima = ;
    float minima = ;
    float period = n;

}