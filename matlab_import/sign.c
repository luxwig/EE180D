/*
 * sign.c
 *
 * Code generation for function 'sign'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "get_feature.h"
#include "sign.h"
#include <stdio.h>

/* Function Definitions */
void b_sign(emxArray_real_T *x)
{
  int nx;
  int k;
  double b_x;
  nx = x->size[0];
  for (k = 0; k + 1 <= nx; k++) {
    if (x->data[k] < 0.0) {
      b_x = -1.0;
    } else if (x->data[k] > 0.0) {
      b_x = 1.0;
    } else if (x->data[k] == 0.0) {
      b_x = 0.0;
    } else {
      b_x = x->data[k];
    }

    x->data[k] = b_x;
  }
}

/* End of code generation (sign.c) */
