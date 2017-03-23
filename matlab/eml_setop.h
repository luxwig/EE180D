/*
 * eml_setop.h
 *
 * Code generation for function 'eml_setop'
 *
 */

#ifndef EML_SETOP_H
#define EML_SETOP_H

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "get_feature_types.h"

/* Function Declarations */
extern void do_vectors(const emxArray_real_T *a, const emxArray_real_T *b,
  emxArray_real_T *c, emxArray_int32_T *ia, emxArray_int32_T *ib);

#endif

/* End of code generation (eml_setop.h) */
