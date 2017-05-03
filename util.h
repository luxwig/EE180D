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
#define _GET_MO_TYPE(X) ((X) & 0xFF0)

/* Function Declarations */

extern int main(int argc, const char * const argv[]);
struct TrainingData_struct
{
    double* m_data;
    int     m_num_data;
    int*    m_divider;
    int     m_num_divider;
    double* m_1st_feature;
    MoType  m_type;
};

typedef struct TrainingData_struct TrainingData;


/*



MoType test_for_walking_speed(double *segment,int length);
void rotate(const double *rotated_data_buf, double *correctly_ordered, int size, int pos);
*/


void mo_training (double*, double* data_fm, size_t n);

void classify_segments(double* correct_data_buf, int pos, int size, MoType* latestMotions, int* latestMotions_num);


void mo_classfication(double*, size_t, MoType*);
int segmentation(const double* data_buf, const int data_buf_size, double* f, size_t* f_num, int* seg, size_t* seg_num, int fntype, double* seg_data);
void train_walk_neural_network(TrainingData all_file_data[], int nFiles);
void read_from_file(const char * filename, double * buffer, size_t* n);


void resample_data(double* raw_data, int* divider, int divider_size, double* resample_data);

    MoType test_for_motion(MoType motion, double *segment, int length, double* first_level_features);

void train_lv2_neural_network(TrainingData all_file_data[], int nFiles, MoType mo_type_lv1,int input_size, int output_size, const MoType* model , const char* fn);
#endif

