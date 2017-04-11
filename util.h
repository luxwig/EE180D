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
#include "statistics.h"

#define _BUFFER 65536
#define _SBUFFER 256
#define RANDOM_BUFFER_MULTIPLIER 8
/* Function Declarations */

extern int main(int argc, const char * const argv[]);
struct TrainingData_struct
{
    double* m_data;
    int     m_num_data;
    int*    m_divider;
    int     m_num_divider;
    MoType  m_type;
};

typedef struct TrainingData_struct TrainingData;


/*



MoType test_for_walking_speed(double *segment,int length);
MoType * classify_segments(double* correct_data_buf, int pos, int size);
void rotate(const double *rotated_data_buf, double *correctly_ordered, int size, int pos);
*/

MoType mo_classfication(double*, size_t, MoType);
void segmentation(const double* data_buf, const int data_buf_size, double* f, int* f_num, int* seg, int* seg_num, int fntype);
void train_walk_neural_network(TrainingData all_file_data[], int nFiles);
void read_from_file(const char * filename, double * buffer, int* n);
#endif

