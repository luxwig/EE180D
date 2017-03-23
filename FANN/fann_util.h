#ifndef FANN_UTIL_H
#define FANN_UTIL_H

#include "fann.h"

void train_data(const char* fn_train, const char* fn_model, const unsigned int num_input, const unsigned int num_output);
void test_data(const char* fn_test, const char* fn_model);

void train_from_data(const char* fn_train, const char* fn_model, const unsigned int num_input, const unsigned int num_outputi, struct fann** ann);

void test_from_data(double* data, int n, struct fann* ann, double* predict);
#endif
