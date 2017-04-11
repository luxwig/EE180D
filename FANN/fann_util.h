#ifndef FANN_UTIL_H
#define FANN_UTIL_H

#include "fann.h"

void train_from_data_float(
        fann_type*   input,
        fann_type*   output,
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

void train_from_file_float(
        fann_type*   input,
        fann_type*   output,
        const unsigned int num_data,
        const unsigned int num_input, 
        const unsigned int num_output,
        const char* fn);

void train_from_file_double(
        double*   input,
        double*   output,
        const unsigned int num_data,
        const unsigned int num_input, 
        const unsigned int num_output,
        const char* fn);


void test_from_data_double(double* data, struct fann*, int n, double* predict);

void test_from_data_float(fann_type* data, struct fann*, int n, fann_type* predict);

void test_from_file_double(double* data, const char* fn, int n, double* predict);

void test_from_file_float(fann_type* data, const char* fn, int n, fann_type* predict);


#endif
