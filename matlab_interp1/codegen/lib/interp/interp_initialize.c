/*
 * File: interp_initialize.c
 *
 * MATLAB Coder version            : 2.6
 * C/C++ source code generated on  : 01-May-2017 17:55:34
 */

/* Include files */
#include "rt_nonfinite.h"
#include "interp.h"
#include "interp_initialize.h"

/* Function Definitions */

/*
 * Arguments    : void
 * Return Type  : void
 */
void interp_initialize(void)
{
  rt_InitInfAndNaN(8U);
}

/*
 * File trailer for interp_initialize.c
 *
 * [EOF]
 */
