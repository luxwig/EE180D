/*
 * zanalysis.h
 *
 * Code generation for function 'zanalysis'
 *
 */

#ifndef ZANALYSIS_H
#define ZANALYSIS_H

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "get_feature_types.h"

/* Function Declarations */
extern void zanalysis(const emxArray_real_T *z_a, emxArray_real_T *val,
                      emxArray_real_T *pos);

#endif

/* End of code generation (zanalysis.h) */
