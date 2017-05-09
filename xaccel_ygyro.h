#ifndef xaccel_h
#define xaccel_h

#include "global.h"
#include <liquid/liquid.h>
#include <stdio.h>
#include <stdlib.h>
#define GRAVITY_OFFSET 1.085 


#define LP_FILTER_ORDER 2 //between 1-5
#define CUTOFF_FREQ_L 5.0f //Hz
#define CUTOFF_FREQ_H 0.1f //Hz
#define SAMPLE_FREQ 10000 //Hz


double get_gravity_offset(double x_accel_data[], int iterations);
void eliminate_offset( double *x_accel_data, int segment_length, double gravity_offset );
void execute_lowpass_filter( double x_accel_data[], int segment_length, double* filtered_data);
void execute_highpass_filter(double input[], int segment_length, double* filtered_data);
void y_gyro_features_2(const double* segment, int segment_length, int begin, int end, double* abs_max, double* rel_min, double* rel_max);
double y_gyro_features_1(const double* segment, int begin, int end);
void get_ygyro(const double* data_buf, const int data_buf_size, double *y_gyro);

#endif