/*
 * _coder_get_feature_api.h
 *
 * Code generation for function '_coder_get_feature_api'
 *
 */

#ifndef _CODER_GET_FEATURE_API_H
#define _CODER_GET_FEATURE_API_H

/* Include files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_get_feature_api.h"

/* Type Definitions */
#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T

struct emxArray_real_T
{
  real_T *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_real_T*/

#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T

typedef struct emxArray_real_T emxArray_real_T;

#endif                                 /*typedef_emxArray_real_T*/

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void get_feature(double, double, emxArray_real_T *m, emxArray_real_T *r, emxArray_real_T *
  features);
extern void get_feature_api(const mxArray *prhs[1], const mxArray *plhs[2]);
extern void get_feature_atexit(void);
extern void get_feature_initialize(void);
extern void get_feature_terminate(void);
extern void get_feature_xil_terminate(void);

#endif

/* End of code generation (_coder_get_feature_api.h) */
