#ifndef GLOBAL_H
#define GLOBAL_H

#define _BUFFER 65536
#define _SBUFFER 256
#define _TRAIN_DATA_SIZE 13
#define _TEST_DATA_SIZE 13
#define _DATA_ACQ_SIZE  8
#define _FIRST_LEVEL_FEATURES 4
#define _MATLAB_OFFSET_FIRST_LEVEL (_FIRST_LEVEL_FEATURES+1)
#define _MATLAB_OFFSET_SECOND_LEVEL 10

#define _TIMESTAMP_BEFORE_OFFSET 0
#define _TIMESTAMP_AFTER_OFFSET 1
#define _ACCEL_X_OFFSET 2
#define _ACCEL_Y_OFFSET 3
#define _ACCEL_Z_OFFSET 4
#define _GYRO_X_OFFSET 5
#define _GYRO_Y_OFFSET 6
#define _GYRO_Z_OFFSET 7

#define _TRUE 1
#define _FALSE 0
static const char * _WALK_NEURAL_NETWORK = "./walk_neural_network.net";
static const char * _MO_NEURAL_NETWORK = "mo_neural_network.net";
static const char TRAINING_DATASET [_TRAIN_DATA_SIZE][_SBUFFER]={
                        "data/WALK/ludwig/1.csv",  
                         "data/WALK/ludwig/2.csv",
                         "data/WALK/ludwig/3.csv",
                         "data/WALK/ludwig/4.csv",
                         "data/RUN/6.csv",  
                         "data/RUN/8.csv",
                         "data/RUN/10.csv",
                         "data/ASCEND/1.csv",
                         "data/ASCEND/2.csv",
                         "data/ASCEND/3.csv",
                         "data/DESCEND/1.csv",
                         "data/DESCEND/2.csv",
                         "data/DESCEND/3.csv",
                   };

static const char TEST_DATASET[_TEST_DATA_SIZE][_SBUFFER]={
                         "data/WALK/ludwig/1_t.csv",  
                         "data/WALK/ludwig/2_t.csv",
                         "data/WALK/ludwig/3_t.csv",
                         "data/WALK/ludwig/4_t.csv",
                         "data/RUN/6_t.csv",  
                         "data/RUN/8_t.csv",
                         "data/RUN/10_t.csv",
                         "data/ASCEND/4.csv",
                         "data/ASCEND/5.csv",
                         "data/ASCEND/6.csv",
                         "data/DESCEND/4.csv",
                         "data/DESCEND/5.csv",
                         "data/DESCEND/6.csv" };   

#define _WALK  0x100
#define _WALK1 0x101
#define _WALK2 0x102
#define _WALK3 0x103
#define _WALK4 0x104
#define _RUN   0x110
#define _RUN1  0x111
#define _RUN2  0x112
#define _RUN3  0x113
#define _RUN4  0x114
#define _ASC   0x210
#define _DSC   0x220

#define _TEST  0x000
#define _TRAINING \
               0xFFF

#define _ASC_DSC_OFFSET         0  // offset for 1lv ASC_DSC mod in result 
#define _WALK_OFFSET            1  // offset for 1lv WALK    mod in result
#define _1ST_LV_ALL_OFFSET      2  // offset for 1lv combime mod in result
#define _WALK_MOD_OFFSET        3  // offset for 2lv WALK    mod in result

#define _FIRST_LEVEL_MOD_COUNT  3  // total 1   lv mod count
#define _TOTAL_MOD_COUNT        4  // total 1+2 lv mod count

enum MoType_enum { TRAINING = _TRAINING,
                   WALK = _WALK, 
                   WALK1 = _WALK1, WALK2 = _WALK2, WALK3 = _WALK3, WALK4 = _WALK4,
                   RUN = _RUN,
                   RUN1 = _RUN1, RUN2 = _RUN2, RUN3 = _RUN3,
                   ASC = _ASC, DSC = _DSC,
                   TEST = _TEST};


typedef enum MoType_enum MoType;

static const MoType fntype[] = {WALK1, WALK2, WALK3, WALK4, ASC, ASC, ASC, DSC, DSC, DSC, RUN1, RUN2, RUN3};

#define _ASC_DSC_SIZE 2
#define _WALK_SIZE 1 
#define _1ST_LV_ALL_SIZE 4
static const MoType ASC_DSC_MODEL[_ASC_DSC_SIZE]={ASC, DSC};
static const MoType WALK_MODEL[_WALK_SIZE] = {WALK};
static const MoType FIRST_LV_ALL_MODEL[_1ST_LV_ALL_SIZE] = {ASC, DSC, WALK, RUN};

static const char* ASC_DSC_FN = "ASC_DSC.net";
static const char* WALK_FN = "WALK.net";
static const char* FIRST_LV_ALL_FN = "FIRST_LV_ALL.net"; 
#endif
