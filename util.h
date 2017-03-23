#ifndef UTIL_H
#define UTIL_H

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "matlab_import/rt_nonfinite.h"
#include "matlab_import/rtwtypes.h"
#include "matlab_import/get_feature_types.h"
#include "FANN/fann_util.h"

#define _BUFFER 65536
#define _SBUFFER 256
/* Function Declarations */

extern int main(int argc, const char * const argv[]);

void segmentation(const char* fn, double* f, int* f_num, int* seg, int* seg_num, int fntype, double* data_val, int* data_len);

enum MoType_enum {WALK1 = 0x001, WALK2 = 0x002, WALK3 = 0x003, WALK4 = 0x004,
                   ASC = 0x0F1, DSC = 0x0F2,
                   TEST = 0xF00};

typedef enum MoType_enum MoType;

void mo_classfication(double*, size_t, MoType);

struct TrainingData_struct
{
    double* m_data;
    int     m_num_data;
    int*    m_divider;
    int     m_num_divider;
    MoType  m_type;
};

typedef struct TrainingData_struct TrainingData;

#endif

