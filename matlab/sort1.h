/*
 * sort1.h
 *
 * Code generation for function 'sort1'
 *
 */

#ifndef SORT1_H
#define SORT1_H

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "get_feature_types.h"

/* Function Declarations */
extern void c_sort(emxArray_real_T *x);
extern void d_sort(emxArray_real_T *x);
extern void f_sort(emxArray_real_T *x);
extern void sort(emxArray_real_T *x, emxArray_int32_T *idx);

#endif

/* End of code generation (sort1.h) */
