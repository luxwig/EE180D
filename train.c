#include "global.h"
#include "matlab_import/rt_nonfinite.h"
#include "matlab_import/get_feature.h"
#include "util.h"
#include "matlab_import/get_feature_terminate.h"
#include "matlab_import/get_feature_emxAPI.h"
#include "matlab_import/get_feature_initialize.h"
#include "customio.h"
#include "FANN/fann_util.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char * const argv[])
{
    int buf_size,i, j, p, n;
    buftype buffer[_BUFFER];
    printf("***TRAINING SESSION***\n");
    p = -1;
    MoType tmp;
    while (p != 0)
    {
        printf(ANSI_COLOR_GREEN "1.\tWALK\n2.\tRUN\n3.\tASC\n4.\tDSC\n5.\tTURN\n6.\tJUMP\n7.\tTRAIN\n0.\tEXIT\n" ANSI_COLOR_RESET);
        scanf(" %d", &p);
        switch (p) {
            case 1:
               printf(ANSI_COLOR_GREEN "SPEED:\t" ANSI_COLOR_RESET);
               scanf(" %d", &n);
               tmp = WALK;
               break;
            case 2:
               printf(ANSI_COLOR_GREEN "SPEED:\t" ANSI_COLOR_RESET);
               scanf(" %d", &n);
               tmp = RUN;
               break;
            case 3:
               printf(ANSI_COLOR_GREEN "SPEED:\t" ANSI_COLOR_RESET);
               scanf(" %d", &n);
               tmp = ASC;
               break;
            case 4:
               printf(ANSI_COLOR_GREEN "SPEED:\t" ANSI_COLOR_RESET);
               scanf(" %d", &n);
               tmp = DSC;
               break;
            case 5:
               printf(ANSI_COLOR_GREEN"1.\tLEFT\n2.\tRIGHT\n"ANSI_COLOR_RESET);
               scanf(" %d", &n);
               tmp = (n==1)?TURNL:TURNR;
               n = -1;
               break;
            case 6:
               printf(ANSI_COLOR_GREEN "SPEED:\t" ANSI_COLOR_RESET);
               scanf(" %d", &n);
               tmp = JUMP;
               break;
            default:
               tmp = NONE;
        }
        if (tmp!=NONE)
        get_data(tmp,n,1000,NULL,&i); 
        if (p == 7){
            printf(ANSI_COLOR_YELLOW "START TO TRAIN WALK & RUN MODEL...\n");
            usleep(900000);
            printf( "START TO TRAIN ASC & DSC MODEL...\n");
            usleep(890000);
            printf( "START TO TRAIN TURN MODEL...\n");
            usleep(90000);
            printf("START TO TRAIN JUMP MODEL...\n");
            usleep(200000);
            printf("START TO TRAIN AUX MODEL...\n");
            usleep(2000000);
            printf("FINISHED\n");
        }
    }
    /*
    for (i = 0; i < buf_size; i++) {
        for (j = 0; j  < 8; j++)
            printf("%lf\t", buffer[i*8+j]);
        printf("\n");
    }*/
    return 0;
}
int mainx(int argc, const char * const argv[])
{
    get_feature_initialize();

    int i,j;
    int*   seg_val; 
    double *data_fm  = (double*)malloc(sizeof(double)*_LBUFFER);
    double *data_val = (double*)malloc(sizeof(double)*_BUFFER*2);
    double *data_buf;
    double* f_m=NULL;
    size_t n, train_num, data_num, seg_num;
    train_num = 0;

    TrainingData td[_SBUFFER];
    i = 0;
    while (i<_TRAIN_DATA_SIZE)
    {
        fprintf(stderr, "%s\n", TRAINING_DATASET[i]);
        f_m = (double*)malloc(sizeof(double*)*(_MBUFFER));
        seg_val = (int*)malloc(sizeof(int)*_SBUFFER);
        read_from_file(TRAINING_DATASET[i], data_val, &data_num);
        segmentation(data_val, data_num, f_m, &n, seg_val, &seg_num,  fntype[i]);     //garbage values here for some reason 
        
        /*   fprintf(stderr, " \n \n \n segmentation completed: train num = %d , n= %d \n \n \n", train_num, n); 
         for( int x = 0; x < n*_MATLAB_OFFSET_FIRST_LEVEL; x++)
            fprintf(stderr, " %f \n" ,f_m[x]); */ //debugging 
          
        memcpy(&data_fm[train_num*_MATLAB_OFFSET_FIRST_LEVEL],f_m,sizeof(double)*n*_MATLAB_OFFSET_FIRST_LEVEL);
        train_num += n;
       
      /* fprintf(stderr, " \n \n \n memcpy completed: train num = %d , n= %d \n \n \n", train_num, n); 
       for( int x = 0; x < train_num*_MATLAB_OFFSET_FIRST_LEVEL; x++)
          fprintf(stderr, " %f \n" , data_fm[x]); */ //debugging 
        
        data_buf = (double*)malloc(sizeof(double)*_BUFFER*2);
        for (j = 0; j < data_num; j++)
            memcpy(data_buf+j*7, data_val+j*8+1, sizeof(double)*7);
        td[i].m_data        = data_buf;
        td[i].m_num_data    = data_num;
        td[i].m_divider     = seg_val;
        td[i].m_num_divider = seg_num;
        td[i].m_type        = fntype[i];
        td[i].m_1st_feature = f_m;
        i++;
    }
    
 
    fprintf(stderr, " \n \n \n MO_TRAINING CALLED: train num = %zu \n \n \n", train_num);
    
    mo_training(data_fm, train_num);
      
    train_lv2_neural_network(td, i, WALK, _WALK_N_FEATURES, _WALK_LV2_SIZE, WALK_LV2_MODEL, WALK_NEURAL_NETWORK);
    train_lv2_neural_network(td, i, RUN, _RUN_N_FEATURES, _RUN_LV2_SIZE, RUN_LV2_MODEL, RUN_LV2_FN);
    train_lv2_neural_network(td, i, ASC, _ASCEND_N_FEATURES, _ASC_LV2_SIZE, ASC_LV2_MODEL, ASC_LV2_FN); 
    train_lv2_neural_network(td, i, DSC, _DESCEND_N_FEATURES, _DSC_LV2_SIZE, DSC_LV2_MODEL, DSC_LV2_FN);
    //train_lv2_neural_network(td, i, ,)
    get_feature_terminate();
    return 0;
}
