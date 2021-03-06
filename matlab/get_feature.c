/*
 * get_feature.c
 *
 * Code generation for function 'get_feature'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "get_feature.h"
#include "get_feature_emxutil.h"
#include "zanalysis.h"
#include "segment.h"

/* Function Definitions */
void get_feature(const emxArray_real_T *m, emxArray_real_T *pos, emxArray_real_T
                 *r, emxArray_real_T *features)
{
  emxArray_real_T *b_m;
  int i0;
  int loop_ub;
  emxArray_real_T *varargin_1;
  emxArray_real_T *varargin_2;
  boolean_T empty_non_axis_sizes;
  int result;
  int b_result;
  int i1;
  emxInit_real_T(&b_m, 2);
  i0 = b_m->size[0] * b_m->size[1];
  b_m->size[0] = m->size[0];
  b_m->size[1] = 7;
  emxEnsureCapacity((emxArray__common *)b_m, i0, (int)sizeof(double));
  loop_ub = m->size[0] * m->size[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    b_m->data[i0] = m->data[i0];
  }

  emxInit_real_T(&varargin_1, 2);
  emxInit_real_T(&varargin_2, 2);
  segment(b_m, pos, r);
  zanalysis(r, varargin_1, varargin_2);
  emxFree_real_T(&b_m);
  if (!((varargin_1->size[0] == 0) || (varargin_1->size[1] == 0))) {
    loop_ub = varargin_1->size[0];
  } else if (!((varargin_2->size[0] == 0) || (varargin_2->size[1] == 0))) {
    loop_ub = varargin_2->size[0];
  } else {
    loop_ub = varargin_1->size[0];
    if (loop_ub >= 0) {
    } else {
      loop_ub = 0;
    }

    if (varargin_2->size[0] > loop_ub) {
      loop_ub = varargin_2->size[0];
    }
  }

  empty_non_axis_sizes = (loop_ub == 0);
  if (empty_non_axis_sizes || (!((varargin_1->size[0] == 0) || (varargin_1->
         size[1] == 0)))) {
    result = varargin_1->size[1];
  } else {
    result = 0;
  }

  if (empty_non_axis_sizes || (!((varargin_2->size[0] == 0) || (varargin_2->
         size[1] == 0)))) {
    b_result = varargin_2->size[1];
  } else {
    b_result = 0;
  }

  i0 = features->size[0] * features->size[1];
  features->size[0] = loop_ub;
  features->size[1] = result + b_result;
  emxEnsureCapacity((emxArray__common *)features, i0, (int)sizeof(double));
  for (i0 = 0; i0 < result; i0++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      features->data[i1 + features->size[0] * i0] = varargin_1->data[i1 +
        loop_ub * i0];
    }
  }

  emxFree_real_T(&varargin_1);
  for (i0 = 0; i0 < b_result; i0++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      features->data[i1 + features->size[0] * (i0 + result)] = varargin_2->
        data[i1 + loop_ub * i0];
    }
  }

  emxFree_real_T(&varargin_2);
}

/* End of code generation (get_feature.c) */
