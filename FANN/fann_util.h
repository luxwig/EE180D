#ifndef FANN_UTIL_H
#define FANN_UTIL_H

#include "fann.h"

void train_data(const char* fn_train, const char* fn_model, const unsigned int num_input, const unsigned int num_output);
void test_data(const char* fn_test, const char* fn_model);

void train_from_data(
        fann_type*   input,
        fann_type*   output,
        const unsigned int num_data,
        const unsigned int num_input, 
        const unsigned int num_output,
        struct fann** ann);

void train_from_file(const char* fn_train, const char* fn_model, const unsigned int num_input, const unsigned int num_outputi, struct fann** ann);

void test_from_data(double* data, struct fann*, int n, double* predict);
#endif
