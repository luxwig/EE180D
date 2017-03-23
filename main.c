/* Include files */
#include "matlab_import/rt_nonfinite.h"
#include "matlab_import/get_feature.h"
#include "util.h"
#include "matlab_import/get_feature_terminate.h"
#include "matlab_import/get_feature_emxAPI.h"
#include "matlab_import/get_feature_initialize.h"

#include "FANN/fann_util.h"

#include <stdio.h>
#include <stdlib.h>

void main_get_feature(void);

#define _FILENUM 20
void main_get_feature(void)
{
  const char fn[_FILENUM][256] = {"data/WALK/ludwig/1.csv",  
                         "data/WALK/ludwig/2.csv",
                         "data/WALK/ludwig/3.csv",
                         "data/WALK/ludwig/4.csv",
                         "data/ASCEND/1.csv",
                         "data/ASCEND/2.csv",
                         "data/ASCEND/3.csv",
                         "data/DESCEND/1.csv",
                         "data/DESCEND/2.csv",
                         "data/DESCEND/3.csv",
                         "data/WALK/ludwig/1_t.csv",  
                         "data/WALK/ludwig/2_t.csv",
                         "data/WALK/ludwig/3_t.csv",
                         "data/WALK/ludwig/4_t.csv",
                         "data/ASCEND/4.csv",
                         "data/ASCEND/5.csv",
                         "data/ASCEND/6.csv",
                         "data/DESCEND/4.csv",
                         "data/DESCEND/5.csv",
                         "data/DESCEND/6.csv" };   
  const int fntype[] = {0x1, 0x1, 0x1, 0x1, 0x2, 0x2, 0x2, 0x3, 0x3, 0x3,
                  0xF0, 0xF0,0xF0, 0xF0, 0xF0, 0xF0, 0xF0,0xF0, 0xF0, 0xF0};
  
  int i, j;
  double data_m [_BUFFER*4];
  double *f_m=NULL;
  size_t n, num, train_num;
  float* input, *output;
  train_num = 0;
  num = 0;

  for (i = 0; i < _FILENUM; i++)
  {
      f_m = (double*)malloc(sizeof(double*)*(_BUFFER));

      get_feature_from_file(fn[i], f_m, (int*)&n, NULL, NULL,  fntype[i]);
      memcpy(&data_m[num*5],f_m,sizeof(double)*n*5);
      if (fntype[i]!=0xF0) train_num += n;
      num+=n;
      free(f_m);
      f_m = NULL;
  }
 
  input  = (float*)malloc(sizeof(float)*train_num*4);
  outpu  = (float*)malloc(sizeof(float)*train_num*3);

  float output_type[3][3]={{1,-1,-1},{-1,1,-1},{-1,-1,1}};
  for (i = 0; i < num; i++) {
     if (data_m[i*5+4] == 0xF0) continue;
     input[i*4] = data_m[i*5], 
     input[i*4+1] = data_m[i*5+1], 
     input[i*4+2] = data_m[i*5+2], 
     input[i*4+3] = data_m[i*5+3];
     memcpy(&output[i*3], output_type[(int)(data_m[i*5+4]-1)], sizeof(float)*3);
     for (j = 0; j < 4; j++)
         fprintf(stderr,"%f ", input[i*4+j]);
     fprintf(stderr,"\n");
     for (j = 0; j < 3; j++)
         fprintf(stderr,"%f ", output[i*3+j]);
     fprintf(stderr,"\n");
  }

  
  struct fann* ann;
  train_from_data(input, output, train_num, 4, 3, &ann);
  double predict[3];
  for (i = 0; i < num; i++)
    if (data_m[i*5+4] == 0xF0) {
        fprintf(stderr, "%lf %lf %lf %lf\n", 
             data_m[i*5], 
             data_m[i*5+1], 
             data_m[i*5+2], 
             data_m[i*5+3]);
        test_from_data(&data_m[i*5], 1, ann, predict);
        fprintf(stderr, "\t%f\t%f\t%f\n",predict[0], predict[1], predict[2]);
        j = predict[0]>predict[1]?0:1;
        j = predict[j]>predict[2]?j:2;
        printf("\t%d\n", j+1);
     }
}

int main(int argc, const char * const argv[])
{
    (void)argc;
    (void)argv;
    
    /* Initialize the application.
     Do not need to do this more than one time. */
    get_feature_initialize();
    
    /* Invoke the entry-point functions.
     Can call entry-point functions multiple times. */
    main_get_feature();
    
    /* Terminate the application.
     Do not need to do this more than one time. */
    get_feature_terminate();
    return 0;
}

