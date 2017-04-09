#ifndef GLOBAL_H
#define GLOBAL_H

#define _BUFFER 65536
#define _SBUFFER 256
#define _TRAIN_DATA_SIZE 10
#define _TEST_DATA_SIZE 10
#define _NUM_DATA_SOURCES 8

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


#endif
