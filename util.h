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

/*
void segmentation(const double* data_buf, const int data_buf_size, double* f, int* f_num, int* seg, int* seg_num, int fntype);


MoType mo_classfication(double*, size_t, MoType);

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
MoType test_for_walking_speed(double *segment,int length);
*/

#endif

