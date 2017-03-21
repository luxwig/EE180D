#ifndef FANN_UTIL_H
#define FANN_UTIL_H

void train_data(const char* fn_train, const char* fn_model, const unsigned int num_input, const unsigned int num_output);
void test_data(const char* fn_test, const char* fn_model);

#endif
