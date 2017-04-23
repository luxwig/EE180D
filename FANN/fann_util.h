#ifndef FANN_UTIL_H
#define FANN_UTIL_H

#include "fann.h"

void train_from_data_float(
        float*   input,
        float*   output,
        const unsigned int num_data,
        const unsigned int num_input, 
        const unsigned int num_output,
        struct fann** ann);

void train_from_data_double(
        double*   input,
        double*   output,
        const unsigned int num_data,
        const unsigned int num_input, 
        const unsigned int num_output,
        struct fann** ann);

void test_from_data_double(double* data, struct fann*, int n, double* predict);

void test_from_data_float(float* data, struct fann*, int n, float* predict);


#endif
