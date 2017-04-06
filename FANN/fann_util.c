#include "fann_util.h"

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "floatfann.h"
#include "fann_train.h"

void train_from_data_double(
        double*   input, //contiguous features one array
        double*   output, //what each contiguous feature should output
        const unsigned int num_data, //how many intervals
        const unsigned int num_input, //number of features
        const unsigned int num_output, //number of options
        struct fann** ann)
{
    int i, j;
    float* f_input  = (float*)malloc(sizeof(float)*num_data*num_input),
         * f_output = (float*)malloc(sizeof(float)*num_data*num_output);
    for (i = 0; i < num_data; i++)
    {
        for (j = 0; j < num_input;j++)
            f_input[i*num_input + j] = (float)input[i*num_input + j];
        for (j = 0; j < num_output;j++)
            f_output[i*num_output + j] = (float)output[i*num_output + j];
    }
    train_from_data_float(f_input, f_output, num_data, num_input, num_output, ann);
}



void train_from_data_float(
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

void test_from_data_double(double* data, struct fann* ann, int n, double* predict)
{
    int i,j,
    num_input  = fann_get_num_input(ann),
    num_output = fann_get_num_output(ann);
    fann_type *f_predict, *f_data;
    f_data = (fann_type*)malloc(sizeof(fann_type) * num_input);
    f_predict = (fann_type*)malloc(sizeof(fann_type) * num_output);

    for(i = 0; i < n; i++)
        for (j = 0; j < num_input; j++)
            f_data[i*num_input+j] = data[i*num_input+j];
    test_from_data_float(f_data, ann, n, f_predict);
    for(i = 0; i < n; i++)
        for (j = 0; j < num_output; j++)
            predict[i*num_output+j] = f_predict[i*num_output+j];
}


void test_from_data_float(float* data, struct fann* ann, int n, float* predict)
{
    int i,
    num_input  = fann_get_num_input(ann),
    num_output = fann_get_num_output(ann);
    fann_type *calc_out;
    for(i = 0; i < n; i++){
        calc_out = fann_run(ann, &data[i*num_input]);
        memcpy(&predict[i*num_output], calc_out, sizeof(fann_type)*num_output); 
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

