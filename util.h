#ifndef UTIL_H
#define UTIL_H

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "matlab_import/rt_nonfinite.h"
#include "matlab_import/rtwtypes.h"
#include "matlab_import/get_feature_types.h"
#include "FANN/fann_util.h"
#include "statistics.h"

#define _BUFFER 65536
#define _SBUFFER 256
/* Function Declarations */

extern int main(int argc, const char * const argv[]);

void segmentation(const char* fn, double* f, int* f_num, int* seg, int* seg_num, int fntype, double* data_val, int* data_len);

enum MoType_enum { TRAINING = 0x000,
                   WALK1 = 0x001, WALK2 = 0x002, WALK3 = 0x003, WALK4 = 0x004,
                   ASC = 0x0F1, DSC = 0x0F2, 
                   ASC1 = 0x0F3, ASC2 = 0x0F4,
                   DSC1 = 0x0F5, DSC2 = 0x0F6,
				   RUN = 0x005, //, 0x006, 0x007, //added Run... 
                   TEST = 0xF00};

typedef enum MoType_enum MoType;

MoType mo_classfication(double* data_fm, size_t n, MoType fntype);

struct TrainingData_struct
{
    double* m_data;
    int     m_num_data;
    int*    m_divider;
    int     m_num_divider;
    MoType  m_type;
};

typedef struct TrainingData_struct TrainingData;

void train_walk_neural_network(TrainingData all_file_data[], int nFiles);
void train_asc_neural_network(TrainingData all_file_data[], int nFiles);
void train_dsc_neural_network(TrainingData all_file_data[], int nFiles);
 
MoType test_for_walking_speed(double *segment,int length);
MoType test_for_asc_speed(double *segment,int length);
MoType test_for_dsc_speed(double *segment,int length);

int findinterval(TrainingData file);
void insert_interval_training(size_t train_num, double* data_fm, TrainingData td[], const MoType fntype[], float sd_each_segment[]);
void insert_interval_testing(size_t n, double* f_m, float sd_each_segment[]);
#endif

