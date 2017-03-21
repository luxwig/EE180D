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
  const int fntype[] = {0x1, 0x1, 0x1, 0x1, 0x2, 0x2, 0x2, 0x4, 0x4, 0x4,
                  0xF0, 0xF0,0xF0, 0xF0, 0xF0, 0xF0, 0xF0,0xF0, 0xF0, 0xF0};
  
  int i;
  double data_m [_BUFFER*8];
  double *f_m=NULL;
  size_t n, num, train_num;
  
  train_num = 0;
  num = 0;

  FILE *fd; 
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
  char* fn_train = "train.txt";
  char* fn_test = "test.txt";
  fd = fopen(fn_train, "w");
  FILE* ft = fopen(fn_test, "w");
  fprintf(fd, "%zu 4 3\n", train_num);
  for (i = 0; i < num; i++) {
     if (data_m[i*5+4] == 0xF0)
        fprintf(ft, "%f %f %f %f\n", 
             data_m[i*5], 
             data_m[i*5+1], 
             data_m[i*5+2], 
             data_m[i*5+3]);
     else {
     fprintf(fd, "%f %f %f %f\n", 
             data_m[i*5], 
             data_m[i*5+1], 
             data_m[i*5+2], 
             data_m[i*5+3]);
     if (data_m[i*5+4] == 0x1)
         fprintf(fd, "1 -1 -1\n");
     if (data_m[i*5+4] == 0x2)
         fprintf(fd, "-1 1 -1\n");
     if (data_m[i*5+4] == 0x4)
         fprintf(fd, "-1 -1 1\n");}
  }
  fclose(fd); fclose(ft);
  train_data(fn_train, "feature.net", 4, 3);
  test_data(fn_test,"feature.net");
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

