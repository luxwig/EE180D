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

#define _FILENUM 42										//changed to 34
#define RANDOM_BUFFER_MULTIPLIER 5						
void main_get_feature(void)											//added the RUN data and ASC/DSC speed
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
                         "data/ASC_SPEED/asc_1.csv",
                         "data/ASC_SPEED/asc_1_2.csv",
                         "data/ASC_SPEED/asc_2_1.csv",
                         "data/ASC_SPEED/asc_2_2.csv",
                         "data/DSC_SPEED/dsc_1.csv",
                         "data/DSC_SPEED/dsc_1_2.csv",
                         "data/DSC_SPEED/dsc_2_1.csv",
                         "data/DSC_SPEED/dsc_2_2.csv",
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
						             "data/RUN/10_t.csv",
                         "data/ASC_SPEED/asc_1_1t.csv",
                         "data/ASC_SPEED/asc_1_2t.csv",
                         "data/ASC_SPEED/asc_2_1t.csv",
                         "data/ASC_SPEED/asc_2_2t.csv",
                         "data/DSC_SPEED/dsc_1_1t.csv",
                         "data/DSC_SPEED/dsc_1_2t.csv",
                         "data/DSC_SPEED/dsc_2_1t.csv",
                         "data/DSC_SPEED/dsc_2_2t.csv"                             
							};   
  const MoType fntype[] = {WALK1, WALK2, WALK3, WALK4, ASC, ASC, ASC, DSC, DSC, DSC, RUN, RUN, RUN, ASC1, ASC1, ASC2, ASC2, DSC1, DSC1, DSC2, DSC2,		
                  TEST, TEST,TEST, TEST, TEST, TEST, TEST,TEST, TEST, TEST, TEST, TEST, TEST, TEST, TEST, TEST, TEST, TEST, TEST, TEST, TEST};
 
  MoType mt;
  int i,j;
  int*   seg_val; 
  double data_fm [_BUFFER*RANDOM_BUFFER_MULTIPLIER];
  double* data_val;
  double *f_m=NULL;
  size_t n, train_num, data_num, seg_num;
  train_num = 0;

  float sd_each_segment[_SBUFFER];           //for sd feature 
  int segment_count = 0; 
  TrainingData td[_SBUFFER];
  i = 0;
  while (i<_FILENUM && fntype[i] != TEST)     //changed from filenum for testing 
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
  
   //grab SD and put it in array to swap with interval 
  float segment_array[_BUFFER*3];
  for(int x = 0; x < td[i].m_num_divider - 1 ; x++)
  {
    for(int y = td[i].m_divider[x]; y < td[i].m_divider[x+1]; y++)
    {
     segment_array[y] = (float)td[i].m_data[y];              //assign values into segment array 
    }
                                       //testing
    
   sd_each_segment[x + segment_count] = w_calculateSD(segment_array, td[i].m_divider[x+1]-td[i].m_divider[x]); 
   
   fprintf(stderr, "sd %d of set %d: %f \n", x, i, sd_each_segment[x+segment_count]);
   
  }
    segment_count += n;
   
   /* for(int z = 0; z < td[i].m_num_divider - 1; z++)
      fprintf(stderr,  "sd: %f \n", sd_each_segment[z]);  //testing */
    
     
  //get z gyro and y gyro data
  //fft segment sets (declare some variable above to hold) insert later down where insert_interval_training is 
  //multiply
  //reverse fft
  //get rms mean and standard deviation   or normal mean, try both 
      i++;
  }  
  
  for(int z = 0; z < segment_count; z++)
  {
  fprintf(stderr,  "sd_%d: %f \n", z, sd_each_segment[z]);  //testing 
  }
  insert_interval_training(train_num, data_fm, td, fntype, sd_each_segment);
  for(int z = 0; z < train_num ;z++)
  {
  fprintf(stderr, "%f \t", data_fm[z*6 + 4]); //testing for sd in right spot  
  }
   //testing 
   fprintf(stderr, "\n");
  //exit(1); 

  mo_classfication(data_fm, train_num, TRAINING);
  train_walk_neural_network(td, i);
  train_asc_neural_network(td, i);
  train_dsc_neural_network(td, i); 


  int k = 0;
  double data_fm2 [_BUFFER*RANDOM_BUFFER_MULTIPLIER];
  
  for (;i<_FILENUM;i++)
  {
      f_m = (double*)malloc(sizeof(double*)*(_BUFFER)); 
      seg_val = (int*)malloc(sizeof(int)*_SBUFFER);
      data_val = (double*)malloc(sizeof(double)*_BUFFER*2);
      segmentation(fn[i], f_m, (int*)&n, seg_val, (int*)&seg_num, fntype[i], data_val, (int*)&data_num);
	  
	  memcpy(&data_fm2[0], f_m, sizeof(double)*n * 5); //copy into array for each file 
 float segment_array[_BUFFER*2];
  for(int x = 0; x < seg_num - 1 ; x++)
  {
    for(int y = seg_val[x]; y < seg_val[x+1]; y++)
    {
     segment_array[y] = (float)data_val[y];              //assign values into segment array 
    }
   sd_each_segment[x] = w_calculateSD(segment_array, seg_val[x+1]-seg_val[x]); 
  }
  
    /*for(int z = 0; z < seg_num - 1; z++)
      fprintf(stderr,  "sd: %f \n", sd_each_segment[z]);  //testing 
                        exit(1);     */                                                    //when i exit here, neural network is a boss... idk why ???
   
	  insert_interval_testing(n, data_fm2, sd_each_segment); //insert tao 
	  for (j = 0; j < n; j++) {
            fprintf(stderr, "%lf %lf %lf %lf %lf %lf\n", 
                 data_fm2[j*5], 
                 data_fm2[j*5+1], 
                 data_fm2[j*5+2], 
                 data_fm2[j*5+3],
				         data_fm2[j*5+4],
                 data_fm2[j*6+4]);

            mt = mo_classfication(&data_fm2[j*6], n, TEST);		//this is giving me trouble
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
            else if(mt == ASC)
            {
                int begin = seg_val[j];
                int end = seg_val[j+1];
                double *dp =(double*)malloc(sizeof(double)*(end-begin+1)); 
                for (k = begin; k<=end; k++)
                {
                    dp[k-begin] = data_val[k*7+1];
                } 
                mt = test_for_asc_speed(dp,end-begin+1);
                if(mt == ASC1)
                  printf(" -> 1");
                else
                  printf(" -> 2");
            }
            else if(mt == DSC)
            {
                int begin = seg_val[j];
                int end = seg_val[j+1];
                double *dp =(double*)malloc(sizeof(double)*(end-begin+1)); 
                for (k = begin; k<=end; k++)
                {
                    dp[k-begin] = data_val[k*7+1];
                } 
                mt = test_for_dsc_speed(dp,end-begin+1);
                 if(mt == DSC1)
                  printf(" -> 1");
                else
                  printf(" -> 2");
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

