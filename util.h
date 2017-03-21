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

#define _BUFFER 65536

/* Function Declarations */
extern int main(int argc, const char * const argv[]);

void get_feature_from_file(const char* fn, double* f, int* f_num, double* seg, int* seg_num, int fntype);

#endif

