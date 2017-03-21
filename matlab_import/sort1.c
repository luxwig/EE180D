/*
 * sort1.c
 *
 * Code generation for function 'sort1'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "get_feature.h"
#include "sort1.h"
#include "get_feature_emxutil.h"
#include "sortIdx.h"
#include <stdio.h>

/* Function Declarations */
static void b_sort(emxArray_real_T *x, int dim, emxArray_int32_T *idx);
static void e_sort(emxArray_real_T *x);

/* Function Definitions */
static void b_sort(emxArray_real_T *x, int dim, emxArray_int32_T *idx)
{
  int i10;
  emxArray_real_T *vwork;
  int vstride;
  int x_idx_0;
  int j;
  emxArray_int32_T *iidx;
  if (dim <= 1) {
    i10 = x->size[0];
  } else {
    i10 = 1;
  }

  emxInit_real_T1(&vwork, 1);
  vstride = vwork->size[0];
  vwork->size[0] = i10;
  emxEnsureCapacity((emxArray__common *)vwork, vstride, (int)sizeof(double));
  x_idx_0 = x->size[0];
  vstride = idx->size[0];
  idx->size[0] = x_idx_0;
  emxEnsureCapacity((emxArray__common *)idx, vstride, (int)sizeof(int));
  vstride = 1;
  x_idx_0 = 1;
  while (x_idx_0 <= dim - 1) {
    vstride *= x->size[0];
    x_idx_0 = 2;
  }

  j = 0;
  emxInit_int32_T(&iidx, 1);
  while (j + 1 <= vstride) {
    for (x_idx_0 = 0; x_idx_0 + 1 <= i10; x_idx_0++) {
      vwork->data[x_idx_0] = x->data[j + x_idx_0 * vstride];
    }

    sortIdx(vwork, iidx);
    for (x_idx_0 = 0; x_idx_0 + 1 <= i10; x_idx_0++) {
      x->data[j + x_idx_0 * vstride] = vwork->data[x_idx_0];
      idx->data[j + x_idx_0 * vstride] = iidx->data[x_idx_0];
    }

    j++;
  }

  emxFree_int32_T(&iidx);
  emxFree_real_T(&vwork);
}

static void e_sort(emxArray_real_T *x)
{
  emxArray_real_T *vwork;
  int i12;
  int k;
  int i13;
  emxArray_int32_T *b_vwork;
  emxInit_real_T1(&vwork, 1);
  i12 = x->size[1];
  k = x->size[1];
  i13 = vwork->size[0];
  vwork->size[0] = k;
  emxEnsureCapacity((emxArray__common *)vwork, i13, (int)sizeof(double));
  for (k = 0; k + 1 <= i12; k++) {
    vwork->data[k] = x->data[k];
  }

  emxInit_int32_T(&b_vwork, 1);
  sortIdx(vwork, b_vwork);
  k = 0;
  emxFree_int32_T(&b_vwork);
  while (k + 1 <= i12) {
    x->data[k] = vwork->data[k];
    k++;
  }

  emxFree_real_T(&vwork);
}

void c_sort(emxArray_real_T *x)
{
  int dim;
  int i11;
  emxArray_real_T *vwork;
  int j;
  int vstride;
  int k;
  emxArray_int32_T *b_vwork;
  dim = 2;
  if (x->size[0] != 1) {
    dim = 1;
  }

  if (dim <= 1) {
    i11 = x->size[0];
  } else {
    i11 = 1;
  }

  emxInit_real_T1(&vwork, 1);
  j = vwork->size[0];
  vwork->size[0] = i11;
  emxEnsureCapacity((emxArray__common *)vwork, j, (int)sizeof(double));
  vstride = 1;
  k = 1;
  while (k <= dim - 1) {
    vstride *= x->size[0];
    k = 2;
  }

  j = 0;
  emxInit_int32_T(&b_vwork, 1);
  while (j + 1 <= vstride) {
    for (k = 0; k + 1 <= i11; k++) {
      vwork->data[k] = x->data[j + k * vstride];
    }

    b_sortIdx(vwork, b_vwork);
    for (k = 0; k + 1 <= i11; k++) {
      x->data[j + k * vstride] = vwork->data[k];
    }

    j++;
  }

  emxFree_int32_T(&b_vwork);
  emxFree_real_T(&vwork);
}

void d_sort(emxArray_real_T *x)
{
  e_sort(x);
}

void f_sort(emxArray_real_T *x)
{
  emxArray_real_T *vwork;
  int i14;
  int k;
  int i15;
  emxArray_int32_T *b_vwork;
  emxInit_real_T1(&vwork, 1);
  i14 = x->size[1];
  k = x->size[1];
  i15 = vwork->size[0];
  vwork->size[0] = k;
  emxEnsureCapacity((emxArray__common *)vwork, i15, (int)sizeof(double));
  for (k = 0; k + 1 <= i14; k++) {
    vwork->data[k] = x->data[k];
  }

  emxInit_int32_T(&b_vwork, 1);
  b_sortIdx(vwork, b_vwork);
  k = 0;
  emxFree_int32_T(&b_vwork);
  while (k + 1 <= i14) {
    x->data[k] = vwork->data[k];
    k++;
  }

  emxFree_real_T(&vwork);
}

void sort(emxArray_real_T *x, emxArray_int32_T *idx)
{
  int dim;
  dim = 2;
  if (x->size[0] != 1) {
    dim = 1;
  }

  b_sort(x, dim, idx);
}

/* End of code generation (sort1.c) */
