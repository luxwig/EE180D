/*
 * findpeaks.h
 *
 * Code generation for function 'findpeaks'
 *
 */

#ifndef FINDPEAKS_H
#define FINDPEAKS_H

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "get_feature_types.h"

/* Function Declarations */
extern void b_findpeaks(const double Yin[200], emxArray_real_T *Ypk,
  emxArray_real_T *Xpk);
extern void findpeaks(const emxArray_real_T *Yin, emxArray_real_T *Ypk,
                      emxArray_real_T *Xpk);

#endif

/* End of code generation (findpeaks.h) */
