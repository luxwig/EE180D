/*
 * File: interp_emxutil.h
 *
 * MATLAB Coder version            : 2.6
 * C/C++ source code generated on  : 02-May-2017 14:48:05
 */

#ifndef __INTERP_EMXUTIL_H__
#define __INTERP_EMXUTIL_H__

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "interp_types.h"

/* Function Declarations */
extern void emxEnsureCapacity(emxArray__common *emxArray, int oldNumel, int
  elementSize);
extern void emxFree_real_T(emxArray_real_T **pEmxArray);
extern void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);

#endif

/*
 * File trailer for interp_emxutil.h
 *
 * [EOF]
 */
