#ifndef UTIL_H
#define UTIL_H
#define _GNU_SOURCE
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


void mo_training (double* data_fm, size_t n);

void classify_segments(double* correct_data_buf, int pos, int size, MoType* latestMotions, int* latestMotions_num);


void mo_classfication(double*, size_t, MoType*);
int segmentation(const double* data_buf, const int data_buf_size, double* f, size_t* f_num, int* seg, size_t* seg_num, int fntype);
void train_walk_neural_network(TrainingData all_file_data[], int nFiles);
void read_from_file(const char * filename, double * buffer, size_t* n);


MoType test_for_motion(MoType motion, double *segment, int length, double* first_level_features);


void train_lv2_neural_network(TrainingData all_file_data[], int nFiles, MoType mo_type_lv1,int input_size, int output_size, const MoType* model , const char* fn);



//xaccel_ygyro function 
#define GRAVITY_OFFSET 1.085 


#define LP_FILTER_ORDER 2 //between 1-5
#define CUTOFF_FREQ_L 5.0f //Hz
#define CUTOFF_FREQ_H 0.1f //Hz
#define SAMPLE_FREQ 10000 //Hz


double get_gravity_offset(double x_accel_data[], int iterations);
void eliminate_offset( double *x_accel_data, int segment_length, double gravity_offset );
void y_gyro_features_2(const double* segment, int segment_length, int begin, int end, double* abs_max, double* rel_min, double* rel_max);
double y_gyro_features_1(const double* segment, int begin, int end);
void get_ygyro(const double* data_buf, const int data_buf_size, double *y_gyro);
void create_ygyro_feature_array(int i, double* ygyro_features, double* abs_max, double* rel_min, double* rel_max);


//zaccel_functions 
void get_zaccel(const double* data_val, const int data_buf_size, double *z_accel);
void z_accel_features_2(const double* segment, int segment_length, int begin, int end, double* abs_max, double* z_accel_at_peak);
double z_accel_features_1( const double* segment, int begin, int end);
void create_zaccel_feature_array(int i, double* zaccel_features, double* abs_max, double* z_accel_at_peak, double* abs_min);


//xgyro_functions
void get_xgyro(const double* data_val, const int data_buf_size, double *x_gyro);
void x_gyro_features_2(const double* segment, int segment_length, int begin, int end, double* abs_max, double* x_gyro_at_peak, double* x_gyro_mean, double* x_gyro_rms, double* x_gyro_kurt);
double x_gyro_features_1( const double* segment, int begin, int end);
void create_xgyro_feature_array(int i, double* xgyro_features, double* abs_max, double* x_gyro_at_peak, double* x_gyro_mean, double* x_gyro_rms, double* x_gyro_kurt);

 void create_xaccel_feature_array(int i, double* xaccel_features, double* x_accel_mean, double* x_accel_std);
 void x_accel_features_2(const double* x_accel, const double* z_gyro, int segment_length, int begin, int end, double* x_accel_mean, double* x_accel_std);
 void get_xaccel(const double* data_val, const int data_buf_size, double* x_accel);
 void get_zgyro(const double* data_val, const int data_buf_size, double* z_gyro); 
#endif

