/* 
 * File: _coder_interp_api.h 
 *  
 * MATLAB Coder version            : 2.6 
 * C/C++ source code generated on  : 01-May-2017 17:55:34 
 */

#ifndef ___CODER_INTERP_API_H__
#define ___CODER_INTERP_API_H__
/* Include files */
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"

/* Type Definitions */
#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T
struct emxArray_real_T
{
    double *data;
    int *size;
    int allocatedSize;
    int numDimensions;
    boolean_T canFreeData;
};
#endif /*struct_emxArray_real_T*/
#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T
typedef struct emxArray_real_T emxArray_real_T;
#endif /*typedef_emxArray_real_T*/

/* Function Declarations */
extern void interp_initialize(emlrtContext *aContext);
extern void interp_terminate(void);
extern void interp_atexit(void);
extern void interp_api(const mxArray *prhs[3], const mxArray *plhs[1]);
extern void interp(emxArray_real_T *x, emxArray_real_T *y, int sample_rate, emxArray_real_T *p);
extern void interp_xil_terminate(void);

#endif
/* 
 * File trailer for _coder_interp_api.h 
 *  
 * [EOF] 
 */
