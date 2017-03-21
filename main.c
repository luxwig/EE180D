/* Include files */
#include "matlab_import/rt_nonfinite.h"
#include "matlab_import/get_feature.h"
#include "main.h"
#include "matlab_import/get_feature_terminate.h"
#include "matlab_import/get_feature_emxAPI.h"
#include "matlab_import/get_feature_initialize.h"

#include "FANN/fann_util.h"

#include <stdio.h>
#include <stdlib.h>

#define _BUFFER 65536
static emxArray_real_T *argInit_Unboundedx7_real_T(void);
static double argInit_real_T(void);
static void main_get_feature(void);

static double argInit_real_T(void)
{
    return 0.0;
}

#define get_index(X,Y,N) X+N*Y
#define _FILENUM 20
static void main_get_feature(void)
{
  char fn[_FILENUM][256] = {"data/WALK/ludwig/1.csv",  
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
  int fntype[] = {0x1, 0x1, 0x1, 0x1, 0x2, 0x2, 0x2, 0x4, 0x4, 0x4,
                  0xF0, 0xF0,0xF0, 0xF0, 0xF0, 0xF0, 0xF0,0xF0, 0xF0, 0xF0};
  int i, j, k;
  double data[_BUFFER*2], data_r[_BUFFER*2];
  double data_m [_BUFFER*8];
  char* line = NULL;
  double temp;
  size_t read, len, n, num, train_num;
  
  train_num = 0;
  emxArray_real_T *r;
  emxArray_real_T *features;
  emxArray_real_T *m;
  num = 0;
  
  FILE *fd;
  for (i = 0; i < _FILENUM; i++)
  {
    fd  = fopen(fn[i],"r");
    getline(&line, &len, fd);
    getline(&line, &len, fd);

    n = 0;
    while ((read = getline(&line, &len, fd))!=-1){
        sscanf(line,"%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
               &temp, &data[n*7], &data[n*7+1], &data[n*7+2],
               &data[n*7+3], &data[n*7+4], &data[n*7+5],&data[n*7+6]);
        n++;
    }

    fclose(fd);
    n--;
    for (k = 0; k < n; k++)
        for (j = 0 ; j<7; j++)
            data_r[get_index(k,j,n)] = data[k*7+j];
            
    emxInitArray_real_T(&r, 2);
    emxInitArray_real_T(&features, 2);

    /* Initialize function 'get_feature' input arguments. */
    /* Initialize function input argument 'm'. */

    m = emxCreateWrapper_real_T(data_r,n,7);

    /* Call the entry-point 'get_feature'. */
    get_feature(m, r, features);
    printf("%d\n%d %d\n", i, r->size[0], r->size[1]);
    for (j = 0; j < features->size[0]; j++) {
        for (k = 0; k < 4; k++) 
            data_m[num*5+k] = features->data[get_index(j,k,features->size[0])];
        data_m[num*5+4] = fntype[i];
        if (fntype[i] != 0xF0) train_num++;
        printf("\t%lf %lf %lf %lf\n", 
                /*
                features->data[get_index(j,0,features->size[0])],
                features->data[get_index(j,1,features->size[0])],
                features->data[get_index(j,2,features->size[0])],
                features->data[get_index(j,3,features->size[0])]);*/
            data_m[num*5], data_m[num*5+1], data_m[num*5+2], data_m[num*5+3]);
        num++;
    }
    emxDestroyArray_real_T(features);
    emxDestroyArray_real_T(r);
    emxDestroyArray_real_T(m);
    fclose(fd);
  }
  char* fn_train = "train.txt";
  char* fn_test = "test.txt";
  fd = fopen(fn_train, "w");
  FILE* ft = fopen(fn_test, "w");
  fprintf(fd, "%d 4 3\n", train_num);
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
  train_data(fn_train, "feature.net");
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

