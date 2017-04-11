#ifndef GLOBAL_H
#define GLOBAL_H

#define _BUFFER 65536
#define _SBUFFER 256
#define _TRAIN_DATA_SIZE 10
#define _TEST_DATA_SIZE 10
#define _DATA_ACQ_SIZE  8

#define _TIMESTAMP_BEFORE_OFFSET 0
#define _TIMESTAMP_AFTER_OFFSET 1
#define _ACCEL_X_OFFSET 2
#define _ACCEL_Y_OFFSET 3
#define _ACCEL_Z_OFFSET 4
#define _GYRO_X_OFFSET 5
#define _GYRO_Y_OFFSET 6
#define _GYRO_Z_OFFSET 7

const char * _WALK_NEURAL_NETWORK = "./walk_neural_network.net";
const char * _MO_NEURAL_NETWORK = "mo_neural_network.net";
const char TRAINING_DATASET [_TRAIN_DATA_SIZE][_SBUFFER]={"data/WALK/ludwig/1.csv",  
                         "data/WALK/ludwig/2.csv",
                         "data/WALK/ludwig/3.csv",
                         "data/WALK/ludwig/4.csv",
                         "data/ASCEND/1.csv",
                         "data/ASCEND/2.csv",
                         "data/ASCEND/3.csv",
                         "data/DESCEND/1.csv",
                         "data/DESCEND/2.csv",
                         "data/DESCEND/3.csv"};

const char TEST_DATASET[_TEST_DATA_SIZE][_SBUFFER]={
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


enum MoType_enum { TRAINING = 0x000,
                   WALK1 = 0x001, WALK2 = 0x002, WALK3 = 0x003, WALK4 = 0x004,
                   ASC = 0x0F1, DSC = 0x0F2,
                   TEST = 0xF00};

typedef enum MoType_enum MoType;


const MoType fntype[] = {WALK1, WALK2, WALK3, WALK4, ASC, ASC, ASC, DSC, DSC, DSC};
#endif
