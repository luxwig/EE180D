#ifndef GLOBAL_H
#define GLOBAL_H

#define _BUFFER     65536
#define _SBUFFER    256
#define _TRAIN_DATA_SIZE 17   //change to increment amoutn of data
#define _TEST_DATA_SIZE  15
#define _DATA_ACQ_SIZE   8
#define _FIRST_LEVEL_FEATURES       9
#define _MATLAB_OFFSET_FIRST_LEVEL  (_FIRST_LEVEL_FEATURES+1)
#define _MATLAB_OFFSET_SECOND_LEVEL 10

#define _TIMESTAMP_BEFORE_OFFSET    0
#define _TIMESTAMP_AFTER_OFFSET     1
#define _ACCEL_X_OFFSET 2
#define _ACCEL_Y_OFFSET 3
#define _ACCEL_Z_OFFSET 4
#define _GYRO_X_OFFSET  5
#define _GYRO_Y_OFFSET  6
#define _GYRO_Z_OFFSET  7


//walk neural network and run neural network use same number of features          //not sure how these things should change 
#define _WALK_N_FEATURES    (5+4) //second level + first level
#define _RUN_N_FEATURES     (5+4) //second level +  first level 
#define _ASCEND_N_FEATURES  (5+4) //second level
#define _DESCEND_N_FEATURES (5+4) //second level
#define _YGYRO_N_FEATURES 4 //first level 

#define _TRUE   1
#define _FALSE  0
static const char * MO_NEURAL_NETWORK = "mo_neural_network.net";
static const char TRAINING_DATASET [_TRAIN_DATA_SIZE][_SBUFFER]={
                        "data/WALK/ludwig/1.csv",  
                         "data/WALK/ludwig/2.csv",
                         "data/WALK/ludwig/3.csv",
                         "data/WALK/ludwig/4.csv",
                         "data/RUN/6.csv",  
                         "data/RUN/8.csv",
                         "data/RUN/10.csv",
                       /*  "data/ASCEND/1.csv",
                         "data/ASCEND/2.csv",
                         "data/ASCEND/3.csv",*/
                         "data/ASC_SPEED/asc_1.csv",
                         "data/ASC_SPEED/asc_1_2.csv",
                         "data/ASC_SPEED/asc_2_1.csv",
                         "data/ASC_SPEED/asc_2_2.csv",
                   /*      "data/DESCEND/1.csv",
                         "data/DESCEND/2.csv",
                         "data/DESCEND/3.csv",*/
                         "data/DSC_SPEED/dsc_1.csv",
                         "data/DSC_SPEED/dsc_1_2.csv",
                         "data/DSC_SPEED/dsc_2_1.csv",
                         "data/DSC_SPEED/dsc_2_2.csv",
                         "data/TL/turn_left_1.csv",
                         "data/TR/turn_right_1.csv"
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
                         "data/DESCEND/6.csv" ,
                          "data/TL/turn_left_1_t.csv",
                          "data/TR/turn_right_1_t.csv"
                          };   

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
#define _ASC1  0x211
#define _ASC2  0x212
#define _DSC   0x220
#define _DSC1  0x221
#define _DSC2  0x222
#define _TURNL 0x300
#define _TURNR 0x310
#define _JUMP  0x400
#define _NONE  0x000

#define _TEST  0xFF0
#define _TRAINING \
               0xFFF

#define _ASC_DSC_OFFSET         0  // offset for 1lv ASC_DSC     mod in result 
#define _WALK_RUN_OFFSET        1  // offset for 1lv WALK_RUN    mod in result
#define _TURNR_TURNL_MOD_OFFSET 2 //offset for 1st level TURN mod in result
#define _JUMP_MOD_OFFSET        3 //ofset for jump mod in result       
#define _1ST_LV_ALL_OFFSET      4  // offset for 1lv combime     mod in result
#define _WALK_RUN_MOD_OFFSET    5  // offset for 2lv WALK_RUN    mod in result
#define _ASC_DSC_MOD_OFFSET     6  // offset for 2lv ASC_DSC     mod in result


#define _FIRST_LEVEL_MOD_COUNT  5  // total 1   lv mod count
#define _TOTAL_MOD_COUNT        7  // total 1+2 lv mod count

enum MoType_enum { TRAINING = _TRAINING,
                   WALK = _WALK, 
                   WALK1 = _WALK1, WALK2 = _WALK2, WALK3 = _WALK3, WALK4 = _WALK4,
                   RUN = _RUN,
                   RUN1 = _RUN1, RUN2 = _RUN2, RUN3 = _RUN3,
                   ASC = _ASC, 
                   ASC1 = _ASC1, ASC2 = _ASC2,
                   DSC = _DSC,
                   DSC1 = _DSC1, DSC2 = _DSC2,
                   TURNR = _TURNR, TURNL = _TURNL,
                   JUMP = _JUMP,
                   TEST = _TEST,
                   NONE = _NONE};


typedef enum MoType_enum MoType;

static const MoType fntype[] = {WALK1, WALK2, WALK3, WALK4, RUN1, RUN2, RUN3, ASC1, ASC1, ASC2, ASC2, DSC1, DSC1, DSC2, DSC2, TURNL, TURNR};


#define _ASC_DSC_SIZE       3
#define _WALK_RUN_SIZE      3
#define _TURNR_TURNL_SIZE   3 //++
#define _JUMP_SIZE          2 //++ 
#define _1ST_LV_ALL_SIZE    6
#define _WALK_LV2_SIZE      4
#define _RUN_LV2_SIZE       3
#define _ASC_LV2_SIZE       2
#define _DSC_LV2_SIZE       2

#define _MASK_LV1 0xFFFF0
#define _MASK_LV2 0XFFFFF


static const MoType ASC_DSC_MODEL[_ASC_DSC_SIZE] =          {ASC, DSC, NONE};
static const MoType WALK_RUN_MODEL[_WALK_RUN_SIZE] =        {WALK, RUN, NONE};
static const MoType TURNR_TURNL_MODEL[_TURNR_TURNL_SIZE] =  {TURNR, TURNL, NONE};    //++
static const MoType JUMP_MODEL[_JUMP_SIZE] =  {JUMP, NONE};    //++
static const MoType FIRST_LV_ALL_MODEL[_1ST_LV_ALL_SIZE] =  {ASC, DSC, WALK, RUN};
static const MoType RUN_LV2_MODEL[_RUN_LV2_SIZE] =          {RUN1, RUN2, RUN3};
static const MoType WALK_LV2_MODEL[_WALK_LV2_SIZE] =        {WALK1, WALK2, WALK3, WALK4};
static const MoType ASC_LV2_MODEL[_ASC_LV2_SIZE] =          {ASC1, ASC2};
static const MoType DSC_LV2_MODEL[_DSC_LV2_SIZE] =          {DSC1, DSC2};

static const char* ASC_DSC_FN =         "ASC_DSC.net";
static const char* WALK_RUN_FN =        "WALK_RUN.net";
static const char* TURNR_TURNL_FN =     "TURNR_TURNL.net";
static const char* JUMP_FN =            "JUMP.net";
static const char* FIRST_LV_ALL_FN =    "FIRST_LV_ALL.net"; 
static const char* RUN_LV2_FN =         "RUN_LV2.net";
static const char* ASC_LV2_FN =         "ASC_LV2.net";
static const char* DSC_LV2_FN =         "DSC_LV2.net";
static const char* WALK_NEURAL_NETWORK= "./walk_neural_network.net";
#endif
