#include <unistd.h>
#include <stdio.h>
#include "floatfann.h"

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
