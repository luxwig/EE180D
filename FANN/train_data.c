#include "fann_util.h"
#include "fann.h"

void train_data(const char* fn_train, const char* fn_model)
{
    fprintf(stderr, "FILE: %s\n", fn_train);
    const unsigned int num_input = 4;
    const unsigned int num_output = 3;
    const unsigned int num_layers = 3;
    const unsigned int num_neurons_hidden = 14;
    const float desired_error = (const float) 0.0001;
    const unsigned int max_epochs = 20000;
    const unsigned int epochs_between_reports = 100;

    struct fann *ann = fann_create_standard(num_layers, num_input,
        num_neurons_hidden, num_output);

    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

    fann_train_on_file(ann, fn_train, max_epochs,
        epochs_between_reports, desired_error);

    fann_save(ann, fn_model);

    fann_destroy(ann);

}
