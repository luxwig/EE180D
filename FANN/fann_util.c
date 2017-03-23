#include "fann_util.h"

#include <unistd.h>
#include <stdio.h>
#include "floatfann.h"
#include "fann_train.h"

void train_from_data(
        fann_type*   input, //contiguous features one array
        fann_type*   output, //what each contiguous feature should output
        const unsigned int num_data, //how many intervals
        const unsigned int num_input, //number of features
        const unsigned int num_output, //number of options
        struct fann** ann)
{
    const unsigned int num_layers = 3;
    const unsigned int num_neurons_hidden = 14;
    const float desired_error = (const float) 0.0001;
    const unsigned int max_epochs = 20000;
    const unsigned int epochs_between_reports = 100;

    struct fann_train_data* t_data = 
        fann_create_train_array(num_data, num_input, input, num_output, output);

    *ann = fann_create_standard(num_layers, num_input,
        num_neurons_hidden, num_output);

    fann_set_activation_function_hidden(*ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(*ann, FANN_SIGMOID_SYMMETRIC);

    fann_train_on_data(*ann, t_data, max_epochs, epochs_between_reports, desired_error);

}


void train_from_file(const char* fn_train, const char* fn_model, const unsigned int num_input, const unsigned int num_output, struct fann** ann)
{
    fprintf(stderr, "FILE: %s\n", fn_train);
    const unsigned int num_layers = 3;
    const unsigned int num_neurons_hidden = 14;
    const float desired_error = (const float) 0.0001;
    const unsigned int max_epochs = 20000;
    const unsigned int epochs_between_reports = 100;

    *ann = fann_create_standard(num_layers, num_input,
        num_neurons_hidden, num_output);

    fann_set_activation_function_hidden(*ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(*ann, FANN_SIGMOID_SYMMETRIC);

    fann_train_on_file(*ann, fn_train, max_epochs,
        epochs_between_reports, desired_error);

    fann_save(*ann, fn_model);

}

void train_data(const char* fn_train, const char* fn_model, const unsigned int num_input, const unsigned int num_output)
{
    fprintf(stderr, "FILE: %s\n", fn_train);
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

//data one d pointer contiguous feature points
//n -> number of rows you are passing
//trained structure
//predict -> output
void test_from_data(double* data, struct fann* ann, int n, double* predict)
{
    int i,j,
    num_input  = fann_get_num_input(ann),
    num_output = fann_get_num_output(ann);
    fann_type *calc_out, *input;
    input = (fann_type*)malloc(sizeof(fann_type) * num_input);
    // FANN SUCKS <<< THEY USE FLOAT
    for(i = 0; i < n; i++){
        for (j = 0; j < num_input; j++)
            input[j] = data[i*num_input+j];
        calc_out = fann_run(ann, input);
        for (j = 0; j < num_output; j++)
            predict[i*num_output+j] = calc_out[j];
    } 
}

void test_data(const char* fn_test, const char* fn_model)
{
    int i;
    float temp0, temp1, temp2, temp3;
    ssize_t read;
    size_t len =0;
    char * line = NULL;
    fann_type *calc_out;
    fann_type input[4];
    struct fann *ann;
    int maxind = 0;
    FILE * fp;
    fp = fopen(fn_test,"r");

    ann = fann_create_from_file(fn_model);

    while ((read = getline(&line, &len, fp)) != -1){
        sscanf(line, "%f %f %f %f", &temp0, &temp1, &temp2, &temp3);
        input[0] = temp0;
        input[1] = temp1;
        input[2] = temp2;
        input[3] = temp3;
        calc_out = fann_run(ann, input);
        fprintf(stderr,"%s",line);
        fprintf(stderr,"\t%.2f\t%.2f\t%.2f\t\n",calc_out[0],calc_out[1],calc_out[2]);
        maxind = 0;
        for (i = 1; i < 3; i++)
            if (calc_out[i]>calc_out[maxind])
                maxind = i;
        fprintf(stdout,"\t%d\n",maxind+1);
    }
    fann_destroy(ann);
}

