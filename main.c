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

#define _FILENUM 26										//changed to 26
#define RANDOM_BUFFER_MULTIPLIER 5						//should this be changed??
void main_get_feature(void)											//added the RUN data
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
						 "data/RUN/6.csv",
						 "data/RUN/8.csv",
					     "data/RUN/10.csv",
                         "data/WALK/ludwig/1_t.csv",  
                         "data/WALK/ludwig/2_t.csv",
                         "data/WALK/ludwig/3_t.csv",
                         "data/WALK/ludwig/4_t.csv",
                         "data/ASCEND/4.csv",
                         "data/ASCEND/5.csv",
                         "data/ASCEND/6.csv",
                         "data/DESCEND/4.csv",
                         "data/DESCEND/5.csv",
                         "data/DESCEND/6.csv",
						 "data/RUN/6_t.csv",
						 "data/RUN/8_t.csv",
						 "data/RUN/10_t.csv"
							};   
  const MoType fntype[] = {WALK1, WALK2, WALK3, WALK4, ASC, ASC, ASC, DSC, DSC, DSC, RUN, RUN, RUN,		//needs to be added to enum
                  TEST, TEST,TEST, TEST, TEST, TEST, TEST,TEST, TEST, TEST, TEST, TEST, TEST};
 
  MoType mt;
  int i,j;
  int*   seg_val; 
  double data_fm [_BUFFER*RANDOM_BUFFER_MULTIPLIER];
  double* data_val;
  double *f_m=NULL;
  size_t n, train_num, data_num, seg_num;
  train_num = 0;

  TrainingData td[_SBUFFER];
  i = 0;
  while (i<_FILENUM && fntype[i] != TEST)
  {
      f_m = (double*)malloc(sizeof(double*)*(_BUFFER));
      seg_val = (int*)malloc(sizeof(int)*_SBUFFER);
      data_val = (double*)malloc(sizeof(double)*_BUFFER*2);
      segmentation(fn[i], f_m, (int*)&n, seg_val, (int*)&seg_num,  fntype[i], data_val, (int*)&data_num);
      memcpy(&data_fm[train_num*5],f_m,sizeof(double)*n*5);
      train_num += n;
      td[i].m_data        = data_val;
      td[i].m_num_data    = data_num;
      td[i].m_divider     = seg_val;
      td[i].m_num_divider = seg_num;
      td[i].m_type        = fntype[i];
      free(f_m);
      i++;
  }
  //code for inserting interval 

  //shift in the right direction data fm to make space for interval 
  int c,d; 
  for (c = train_num -1; c > 4; c -=5) //at end of each segment 
  {
	  for (d = train_num; d > c;d--)
	  {
		  data_fm[d] = data_fm[d-1]; 
	  }
	  train_num += 1;
  }
  //fill with correct value of segment 
  int* intervals;
  int position = 0;
  for (int i = 0; i < _FILENUM && fntype[i] != TEST; ) //fill array with intervals 
  {
	  for (int j = 0; j < td[i].m_num_divider; j++)
	  {
		  intervals[position + j] = td[i].m_divider[j+1] - td[i].m_divider[j];
	  }
	  position += td[i].m_num_divider; 
  }
  int j = 0;
  //insert interval to array of features
  for (int i = 4; i < train_num; i += 6)
  {
	  data_fm[i] = intervals[j]; 
  }
  //end code for interval insertion 


  mo_classfication(data_fm, train_num, TRAINING);
  train_walk_neural_network(td, i);
  int k = 0;
  for (;i<_FILENUM;i++)
  {
      f_m = (double*)malloc(sizeof(double*)*(_BUFFER));
      seg_val = (int*)malloc(sizeof(int)*_SBUFFER);
      data_val = (double*)malloc(sizeof(double)*_BUFFER*2);
      segmentation(fn[i], f_m, (int*)&n, seg_val, (int*)&seg_num,  fntype[i], data_val, (int*)&data_num);
      for (j = 0; j < n; j++) {
            fprintf(stderr, "%lf %lf %lf %lf\n", 
                 f_m[j*5], 
                 f_m[j*5+1], 
                 f_m[j*5+2], 
                 f_m[j*5+3]);
            mt = mo_classfication(&f_m[j*5], n, TEST);
            printf("%x",mt); 
            if (mt == TRAINING){
                int begin = seg_val[j];
                int end = seg_val[j+1];
                double *dp =(double*)malloc(sizeof(double)*(end-begin+1)); 
                for (k = begin; k<=end; k++)
                {
                    dp[k-begin] = data_val[k*7+1];
                } 
                mt = test_for_walking_speed(dp,end-begin+1);
                printf(" -> %x", mt);
            }
            printf("\n");
      }
      free(f_m);
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

