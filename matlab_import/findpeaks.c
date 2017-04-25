/*
 * findpeaks.c
 *
 * Code generation for function 'findpeaks'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "get_feature.h"
#include "findpeaks.h"
#include "get_feature_emxutil.h"
#include "sort1.h"
#include "eml_setop.h"
#include "diff.h"
#include "sign.h"

/* Function Declarations */
static void b_findLocalMaxima(const double yTemp[200], emxArray_real_T *iPk,
  emxArray_real_T *iInflect);
static void combinePeaks(const emxArray_real_T *iPk, const emxArray_real_T *iInf,
  emxArray_real_T *iPkOut);
static void findLocalMaxima(emxArray_real_T *yTemp, emxArray_real_T *iPk,
  emxArray_real_T *iInflect);
static void parse_inputs(const emxArray_real_T *Yin, emxArray_real_T *y,
  boolean_T *yIsRow, emxArray_real_T *x, boolean_T *xIsRow, double *NpOut);

/* Function Definitions */
static void b_findLocalMaxima(const double yTemp[200], emxArray_real_T *iPk,
  emxArray_real_T *iInflect)
{
  double b_yTemp[202];
  boolean_T yFinite[202];
  int ii;
  boolean_T x[201];
  int idx;
  unsigned char ii_data[201];
  boolean_T exitg3;
  boolean_T guard3 = false;
  int nx;
  unsigned char tmp_data[202];
  int i8;
  int i9;
  double yTemp_data[202];
  unsigned char iTemp_data[202];
  int yTemp_size[1];
  emxArray_real_T *s;
  emxArray_boolean_T *b_x;
  emxArray_real_T *r5;
  emxArray_real_T b_yTemp_data;
  emxArray_int32_T *b_ii;
  boolean_T exitg2;
  boolean_T guard2 = false;
  emxArray_int32_T *c_ii;
  boolean_T exitg1;
  boolean_T guard1 = false;
  b_yTemp[0] = rtNaN;
  memcpy(&b_yTemp[1], &yTemp[0], 200U * sizeof(double));
  b_yTemp[201] = rtNaN;
  for (ii = 0; ii < 202; ii++) {
    yFinite[ii] = !rtIsNaN(b_yTemp[ii]);
  }

  for (ii = 0; ii < 201; ii++) {
    x[ii] = ((b_yTemp[ii] != b_yTemp[ii + 1]) && (yFinite[ii] || yFinite[ii + 1]));
  }

  idx = 0;
  ii = 1;
  exitg3 = false;
  while ((!exitg3) && (ii < 202)) {
    guard3 = false;
    if (x[ii - 1]) {
      idx++;
      ii_data[idx - 1] = (unsigned char)ii;
      if (idx >= 201) {
        exitg3 = true;
      } else {
        guard3 = true;
      }
    } else {
      guard3 = true;
    }

    if (guard3) {
      ii++;
    }
  }

  if (1 > idx) {
    nx = 0;
  } else {
    nx = idx;
  }

  tmp_data[0] = 1;
  for (i8 = 0; i8 < nx; i8++) {
    tmp_data[i8 + 1] = (unsigned char)(ii_data[i8] + 1);
  }

  if (1 > idx) {
    i9 = 0;
  } else {
    i9 = idx;
  }

  ii = 1 + i9;
  for (i8 = 0; i8 < ii; i8++) {
    iTemp_data[i8] = tmp_data[i8];
  }

  yTemp_size[0] = 1 + nx;
  nx++;
  for (i8 = 0; i8 < nx; i8++) {
    yTemp_data[i8] = b_yTemp[iTemp_data[i8] - 1];
  }

  emxInit_real_T1(&s, 1);
  emxInit_boolean_T(&b_x, 1);
  emxInit_real_T1(&r5, 1);
  b_yTemp_data.data = (double *)&yTemp_data;
  b_yTemp_data.size = (int *)&yTemp_size;
  b_yTemp_data.allocatedSize = 202;
  b_yTemp_data.numDimensions = 1;
  b_yTemp_data.canFreeData = false;
  diff(&b_yTemp_data, s);
  b_sign(s);
  diff(s, r5);
  i8 = b_x->size[0];
  b_x->size[0] = r5->size[0];
  emxEnsureCapacity((emxArray__common *)b_x, i8, (int)sizeof(boolean_T));
  nx = r5->size[0];
  for (i8 = 0; i8 < nx; i8++) {
    b_x->data[i8] = (r5->data[i8] < 0.0);
  }

  emxFree_real_T(&r5);
  emxInit_int32_T(&b_ii, 1);
  nx = b_x->size[0];
  idx = 0;
  i8 = b_ii->size[0];
  b_ii->size[0] = b_x->size[0];
  emxEnsureCapacity((emxArray__common *)b_ii, i8, (int)sizeof(int));
  ii = 1;
  exitg2 = false;
  while ((!exitg2) && (ii <= nx)) {
    guard2 = false;
    if (b_x->data[ii - 1]) {
      idx++;
      b_ii->data[idx - 1] = ii;
      if (idx >= nx) {
        exitg2 = true;
      } else {
        guard2 = true;
      }
    } else {
      guard2 = true;
    }

    if (guard2) {
      ii++;
    }
  }

  if (b_x->size[0] == 1) {
    if (idx == 0) {
      i8 = b_ii->size[0];
      b_ii->size[0] = 0;
      emxEnsureCapacity((emxArray__common *)b_ii, i8, (int)sizeof(int));
    }
  } else {
    i8 = b_ii->size[0];
    if (1 > idx) {
      b_ii->size[0] = 0;
    } else {
      b_ii->size[0] = idx;
    }

    emxEnsureCapacity((emxArray__common *)b_ii, i8, (int)sizeof(int));
  }

  if (1.0 > (double)s->size[0] - 1.0) {
    nx = 0;
  } else {
    nx = (int)((double)s->size[0] - 1.0);
  }

  if (2 > s->size[0]) {
    i8 = 0;
  } else {
    i8 = 1;
  }

  ii = b_x->size[0];
  b_x->size[0] = nx;
  emxEnsureCapacity((emxArray__common *)b_x, ii, (int)sizeof(boolean_T));
  for (ii = 0; ii < nx; ii++) {
    b_x->data[ii] = (s->data[ii] != s->data[i8 + ii]);
  }

  emxFree_real_T(&s);
  emxInit_int32_T(&c_ii, 1);
  nx = b_x->size[0];
  idx = 0;
  i8 = c_ii->size[0];
  c_ii->size[0] = b_x->size[0];
  emxEnsureCapacity((emxArray__common *)c_ii, i8, (int)sizeof(int));
  ii = 1;
  exitg1 = false;
  while ((!exitg1) && (ii <= nx)) {
    guard1 = false;
    if (b_x->data[ii - 1]) {
      idx++;
      c_ii->data[idx - 1] = ii;
      if (idx >= nx) {
        exitg1 = true;
      } else {
        guard1 = true;
      }
    } else {
      guard1 = true;
    }

    if (guard1) {
      ii++;
    }
  }

  if (b_x->size[0] == 1) {
    if (idx == 0) {
      i8 = c_ii->size[0];
      c_ii->size[0] = 0;
      emxEnsureCapacity((emxArray__common *)c_ii, i8, (int)sizeof(int));
    }
  } else {
    i8 = c_ii->size[0];
    if (1 > idx) {
      c_ii->size[0] = 0;
    } else {
      c_ii->size[0] = idx;
    }

    emxEnsureCapacity((emxArray__common *)c_ii, i8, (int)sizeof(int));
  }

  emxFree_boolean_T(&b_x);
  i8 = iInflect->size[0];
  iInflect->size[0] = c_ii->size[0];
  emxEnsureCapacity((emxArray__common *)iInflect, i8, (int)sizeof(double));
  nx = c_ii->size[0];
  for (i8 = 0; i8 < nx; i8++) {
    iInflect->data[i8] = (double)iTemp_data[c_ii->data[i8]] - 1.0;
  }

  emxFree_int32_T(&c_ii);
  i8 = iPk->size[0];
  iPk->size[0] = b_ii->size[0];
  emxEnsureCapacity((emxArray__common *)iPk, i8, (int)sizeof(double));
  nx = b_ii->size[0];
  for (i8 = 0; i8 < nx; i8++) {
    iPk->data[i8] = (double)iTemp_data[b_ii->data[i8]] - 1.0;
  }

  emxFree_int32_T(&b_ii);
}

static void combinePeaks(const emxArray_real_T *iPk, const emxArray_real_T *iInf,
  emxArray_real_T *iPkOut)
{
  emxArray_int32_T *ia;
  emxArray_int32_T *ib;
  emxInit_int32_T(&ia, 1);
  emxInit_int32_T(&ib, 1);
  do_vectors(iPk, iInf, iPkOut, ia, ib);
  emxFree_int32_T(&ib);
  emxFree_int32_T(&ia);
}

static void findLocalMaxima(emxArray_real_T *yTemp, emxArray_real_T *iPk,
  emxArray_real_T *iInflect)
{
  emxArray_real_T *r1;
  int i4;
  int cdiff;
  int ndbl;
  int apnd;
  int absb;
  emxArray_real_T *y;
  emxArray_real_T *iTemp;
  emxArray_boolean_T *yFinite;
  emxArray_boolean_T *x;
  emxArray_int32_T *ii;
  boolean_T exitg3;
  boolean_T guard3 = false;
  emxArray_int32_T *r2;
  emxArray_real_T *b_iTemp;
  emxArray_real_T *b_yTemp;
  emxArray_real_T *s;
  emxArray_real_T *r3;
  boolean_T exitg2;
  boolean_T guard2 = false;
  emxArray_int32_T *b_ii;
  boolean_T exitg1;
  boolean_T guard1 = false;
  emxInit_real_T1(&r1, 1);
  i4 = r1->size[0];
  r1->size[0] = 2 + yTemp->size[0];
  emxEnsureCapacity((emxArray__common *)r1, i4, (int)sizeof(double));
  r1->data[0] = rtNaN;
  cdiff = yTemp->size[0];
  for (i4 = 0; i4 < cdiff; i4++) {
    r1->data[i4 + 1] = yTemp->data[i4];
  }

  r1->data[1 + yTemp->size[0]] = rtNaN;
  i4 = yTemp->size[0];
  yTemp->size[0] = r1->size[0];
  emxEnsureCapacity((emxArray__common *)yTemp, i4, (int)sizeof(double));
  cdiff = r1->size[0];
  for (i4 = 0; i4 < cdiff; i4++) {
    yTemp->data[i4] = r1->data[i4];
  }

  emxFree_real_T(&r1);
  ndbl = (int)floor(((double)yTemp->size[0] - 1.0) + 0.5);
  apnd = ndbl + 1;
  cdiff = (ndbl - yTemp->size[0]) + 1;
  absb = yTemp->size[0];
  if (abs(cdiff) < 4.4408920985006262E-16 * (double)absb) {
    ndbl++;
    apnd = yTemp->size[0];
  } else if (cdiff > 0) {
    apnd = ndbl;
  } else {
    ndbl++;
  }

  emxInit_real_T(&y, 2);
  i4 = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = ndbl;
  emxEnsureCapacity((emxArray__common *)y, i4, (int)sizeof(double));
  y->data[0] = 1.0;
  if (ndbl > 1) {
    y->data[ndbl - 1] = apnd;
    i4 = ndbl - 1;
    cdiff = (i4 + (i4 < 0)) >> 1;
    for (absb = 1; absb < cdiff; absb++) {
      y->data[absb] = 1.0 + (double)absb;
      y->data[(ndbl - absb) - 1] = apnd - absb;
    }

    if (cdiff << 1 == ndbl - 1) {
      y->data[cdiff] = (1.0 + (double)apnd) / 2.0;
    } else {
      y->data[cdiff] = 1.0 + (double)cdiff;
      y->data[cdiff + 1] = apnd - cdiff;
    }
  }

  emxInit_real_T1(&iTemp, 1);
  i4 = iTemp->size[0];
  iTemp->size[0] = y->size[1];
  emxEnsureCapacity((emxArray__common *)iTemp, i4, (int)sizeof(double));
  cdiff = y->size[1];
  for (i4 = 0; i4 < cdiff; i4++) {
    iTemp->data[i4] = y->data[y->size[0] * i4];
  }

  emxFree_real_T(&y);
  emxInit_boolean_T(&yFinite, 1);
  i4 = yFinite->size[0];
  yFinite->size[0] = yTemp->size[0];
  emxEnsureCapacity((emxArray__common *)yFinite, i4, (int)sizeof(boolean_T));
  cdiff = yTemp->size[0];
  for (i4 = 0; i4 < cdiff; i4++) {
    yFinite->data[i4] = rtIsNaN(yTemp->data[i4]);
  }

  i4 = yFinite->size[0];
  emxEnsureCapacity((emxArray__common *)yFinite, i4, (int)sizeof(boolean_T));
  cdiff = yFinite->size[0];
  for (i4 = 0; i4 < cdiff; i4++) {
    yFinite->data[i4] = !yFinite->data[i4];
  }

  emxInit_boolean_T(&x, 1);
  cdiff = yTemp->size[0] - 2;
  i4 = x->size[0];
  x->size[0] = cdiff + 1;
  emxEnsureCapacity((emxArray__common *)x, i4, (int)sizeof(boolean_T));
  for (i4 = 0; i4 <= cdiff; i4++) {
    x->data[i4] = ((yTemp->data[i4] != yTemp->data[1 + i4]) && (yFinite->data[i4]
      || yFinite->data[1 + i4]));
  }

  emxFree_boolean_T(&yFinite);
  emxInit_int32_T(&ii, 1);
  absb = x->size[0];
  ndbl = 0;
  i4 = ii->size[0];
  ii->size[0] = x->size[0];
  emxEnsureCapacity((emxArray__common *)ii, i4, (int)sizeof(int));
  cdiff = 1;
  exitg3 = false;
  while ((!exitg3) && (cdiff <= absb)) {
    guard3 = false;
    if (x->data[cdiff - 1]) {
      ndbl++;
      ii->data[ndbl - 1] = cdiff;
      if (ndbl >= absb) {
        exitg3 = true;
      } else {
        guard3 = true;
      }
    } else {
      guard3 = true;
    }

    if (guard3) {
      cdiff++;
    }
  }

  if (x->size[0] == 1) {
    if (ndbl == 0) {
      i4 = ii->size[0];
      ii->size[0] = 0;
      emxEnsureCapacity((emxArray__common *)ii, i4, (int)sizeof(int));
    }
  } else {
    i4 = ii->size[0];
    if (1 > ndbl) {
      ii->size[0] = 0;
    } else {
      ii->size[0] = ndbl;
    }

    emxEnsureCapacity((emxArray__common *)ii, i4, (int)sizeof(int));
  }

  emxInit_int32_T(&r2, 1);
  i4 = r2->size[0];
  r2->size[0] = 1 + ii->size[0];
  emxEnsureCapacity((emxArray__common *)r2, i4, (int)sizeof(int));
  r2->data[0] = 1;
  cdiff = ii->size[0];
  for (i4 = 0; i4 < cdiff; i4++) {
    r2->data[i4 + 1] = ii->data[i4] + 1;
  }

  emxInit_real_T1(&b_iTemp, 1);
  i4 = b_iTemp->size[0];
  b_iTemp->size[0] = r2->size[0];
  emxEnsureCapacity((emxArray__common *)b_iTemp, i4, (int)sizeof(double));
  cdiff = r2->size[0];
  for (i4 = 0; i4 < cdiff; i4++) {
    b_iTemp->data[i4] = iTemp->data[r2->data[i4] - 1];
  }

  emxFree_int32_T(&r2);
  i4 = iTemp->size[0];
  iTemp->size[0] = b_iTemp->size[0];
  emxEnsureCapacity((emxArray__common *)iTemp, i4, (int)sizeof(double));
  cdiff = b_iTemp->size[0];
  for (i4 = 0; i4 < cdiff; i4++) {
    iTemp->data[i4] = b_iTemp->data[i4];
  }

  emxFree_real_T(&b_iTemp);
  emxInit_real_T1(&b_yTemp, 1);
  i4 = b_yTemp->size[0];
  b_yTemp->size[0] = iTemp->size[0];
  emxEnsureCapacity((emxArray__common *)b_yTemp, i4, (int)sizeof(double));
  cdiff = iTemp->size[0];
  for (i4 = 0; i4 < cdiff; i4++) {
    b_yTemp->data[i4] = yTemp->data[(int)iTemp->data[i4] - 1];
  }

  emxInit_real_T1(&s, 1);
  emxInit_real_T1(&r3, 1);
  diff(b_yTemp, s);
  b_sign(s);
  diff(s, r3);
  i4 = x->size[0];
  x->size[0] = r3->size[0];
  emxEnsureCapacity((emxArray__common *)x, i4, (int)sizeof(boolean_T));
  cdiff = r3->size[0];
  emxFree_real_T(&b_yTemp);
  for (i4 = 0; i4 < cdiff; i4++) {
    x->data[i4] = (r3->data[i4] < 0.0);
  }

  emxFree_real_T(&r3);
  absb = x->size[0];
  ndbl = 0;
  i4 = ii->size[0];
  ii->size[0] = x->size[0];
  emxEnsureCapacity((emxArray__common *)ii, i4, (int)sizeof(int));
  cdiff = 1;
  exitg2 = false;
  while ((!exitg2) && (cdiff <= absb)) {
    guard2 = false;
    if (x->data[cdiff - 1]) {
      ndbl++;
      ii->data[ndbl - 1] = cdiff;
      if (ndbl >= absb) {
        exitg2 = true;
      } else {
        guard2 = true;
      }
    } else {
      guard2 = true;
    }

    if (guard2) {
      cdiff++;
    }
  }

  if (x->size[0] == 1) {
    if (ndbl == 0) {
      i4 = ii->size[0];
      ii->size[0] = 0;
      emxEnsureCapacity((emxArray__common *)ii, i4, (int)sizeof(int));
    }
  } else {
    i4 = ii->size[0];
    if (1 > ndbl) {
      ii->size[0] = 0;
    } else {
      ii->size[0] = ndbl;
    }

    emxEnsureCapacity((emxArray__common *)ii, i4, (int)sizeof(int));
  }

  if (1.0 > (double)s->size[0] - 1.0) {
    cdiff = 0;
  } else {
    cdiff = (int)((double)s->size[0] - 1.0);
  }

  if (2 > s->size[0]) {
    i4 = 0;
  } else {
    i4 = 1;
  }

  absb = x->size[0];
  x->size[0] = cdiff;
  emxEnsureCapacity((emxArray__common *)x, absb, (int)sizeof(boolean_T));
  for (absb = 0; absb < cdiff; absb++) {
    x->data[absb] = (s->data[absb] != s->data[i4 + absb]);
  }

  emxFree_real_T(&s);
  emxInit_int32_T(&b_ii, 1);
  absb = x->size[0];
  ndbl = 0;
  i4 = b_ii->size[0];
  b_ii->size[0] = x->size[0];
  emxEnsureCapacity((emxArray__common *)b_ii, i4, (int)sizeof(int));
  cdiff = 1;
  exitg1 = false;
  while ((!exitg1) && (cdiff <= absb)) {
    guard1 = false;
    if (x->data[cdiff - 1]) {
      ndbl++;
      b_ii->data[ndbl - 1] = cdiff;
      if (ndbl >= absb) {
        exitg1 = true;
      } else {
        guard1 = true;
      }
    } else {
      guard1 = true;
    }

    if (guard1) {
      cdiff++;
    }
  }

  if (x->size[0] == 1) {
    if (ndbl == 0) {
      i4 = b_ii->size[0];
      b_ii->size[0] = 0;
      emxEnsureCapacity((emxArray__common *)b_ii, i4, (int)sizeof(int));
    }
  } else {
    i4 = b_ii->size[0];
    if (1 > ndbl) {
      b_ii->size[0] = 0;
    } else {
      b_ii->size[0] = ndbl;
    }

    emxEnsureCapacity((emxArray__common *)b_ii, i4, (int)sizeof(int));
  }

  emxFree_boolean_T(&x);
  i4 = iInflect->size[0];
  iInflect->size[0] = b_ii->size[0];
  emxEnsureCapacity((emxArray__common *)iInflect, i4, (int)sizeof(double));
  cdiff = b_ii->size[0];
  for (i4 = 0; i4 < cdiff; i4++) {
    iInflect->data[i4] = iTemp->data[b_ii->data[i4]] - 1.0;
  }

  emxFree_int32_T(&b_ii);
  i4 = iPk->size[0];
  iPk->size[0] = ii->size[0];
  emxEnsureCapacity((emxArray__common *)iPk, i4, (int)sizeof(double));
  cdiff = ii->size[0];
  for (i4 = 0; i4 < cdiff; i4++) {
    iPk->data[i4] = iTemp->data[ii->data[i4]] - 1.0;
  }

  emxFree_int32_T(&ii);
  emxFree_real_T(&iTemp);
}

static void parse_inputs(const emxArray_real_T *Yin, emxArray_real_T *y,
  boolean_T *yIsRow, emxArray_real_T *x, boolean_T *xIsRow, double *NpOut)
{
  boolean_T b_yIsRow;
  int ndbl;
  int nm1d2;
  int cdiff;
  int apnd;
  emxArray_real_T *b_y;
  b_yIsRow = (Yin->size[0] == 1);
  *yIsRow = b_yIsRow;
  ndbl = y->size[0];
  y->size[0] = Yin->size[0];
  emxEnsureCapacity((emxArray__common *)y, ndbl, (int)sizeof(double));
  nm1d2 = Yin->size[0];
  for (ndbl = 0; ndbl < nm1d2; ndbl++) {
    y->data[ndbl] = Yin->data[ndbl];
  }

  *xIsRow = b_yIsRow;
  nm1d2 = Yin->size[0];
  if (nm1d2 < 1) {
    cdiff = 0;
    apnd = Yin->size[0];
  } else {
    nm1d2 = Yin->size[0];
    ndbl = (int)floor(((double)nm1d2 - 1.0) + 0.5);
    apnd = ndbl + 1;
    nm1d2 = Yin->size[0];
    cdiff = (ndbl - nm1d2) + 1;
    nm1d2 = Yin->size[0];
    if (1 >= nm1d2) {
      nm1d2 = 1;
    }

    if (abs(cdiff) < 4.4408920985006262E-16 * (double)nm1d2) {
      ndbl++;
      apnd = Yin->size[0];
    } else if (cdiff > 0) {
      apnd = ndbl;
    } else {
      ndbl++;
    }

    if (ndbl >= 0) {
      cdiff = ndbl;
    } else {
      cdiff = 0;
    }
  }

  emxInit_real_T(&b_y, 2);
  ndbl = b_y->size[0] * b_y->size[1];
  b_y->size[0] = 1;
  b_y->size[1] = cdiff;
  emxEnsureCapacity((emxArray__common *)b_y, ndbl, (int)sizeof(double));
  if (cdiff > 0) {
    b_y->data[0] = 1.0;
    if (cdiff > 1) {
      b_y->data[cdiff - 1] = apnd;
      ndbl = cdiff - 1;
      nm1d2 = ndbl >> 1;
      for (ndbl = 1; ndbl < nm1d2; ndbl++) {
        b_y->data[ndbl] = 1.0 + (double)ndbl;
        b_y->data[(cdiff - ndbl) - 1] = apnd - ndbl;
      }

      if (nm1d2 << 1 == cdiff - 1) {
        b_y->data[nm1d2] = (1.0 + (double)apnd) / 2.0;
      } else {
        b_y->data[nm1d2] = 1.0 + (double)nm1d2;
        b_y->data[nm1d2 + 1] = apnd - nm1d2;
      }
    }
  }

  ndbl = x->size[0];
  x->size[0] = b_y->size[1];
  emxEnsureCapacity((emxArray__common *)x, ndbl, (int)sizeof(double));
  nm1d2 = b_y->size[1];
  for (ndbl = 0; ndbl < nm1d2; ndbl++) {
    x->data[ndbl] = b_y->data[b_y->size[0] * ndbl];
  }

  emxFree_real_T(&b_y);
  nm1d2 = Yin->size[0];
  *NpOut = nm1d2;
}

void b_findpeaks(const double Yin[200], emxArray_real_T *Ypk, emxArray_real_T
                 *Xpk)
{
  boolean_T x[200];
  int i7;
  int idx;
  int ii_data[200];
  int ii;
  boolean_T exitg1;
  boolean_T guard1 = false;
  int iInfite_size[1];
  double yTemp[200];
  double iInfite_data[200];
  emxArray_real_T *iPk;
  emxArray_real_T *base;
  int trueCount;
  int i;
  double b_iPk;
  emxArray_real_T *c_iPk;
  emxArray_real_T b_iInfite_data;
  emxArray_real_T *c_sortIdx;
  emxArray_int32_T *iidx;
  emxArray_real_T *b_base;
  emxArray_boolean_T *idelete;
  unsigned int unnamed_idx_0;
  emxArray_boolean_T *r4;
  double d_iPk;
  for (i7 = 0; i7 < 200; i7++) {
    x[i7] = (rtIsInf(Yin[i7]) && (Yin[i7] > 0.0));
  }

  idx = 0;
  ii = 1;
  exitg1 = false;
  while ((!exitg1) && (ii < 201)) {
    guard1 = false;
    if (x[ii - 1]) {
      idx++;
      ii_data[idx - 1] = ii;
      if (idx >= 200) {
        exitg1 = true;
      } else {
        guard1 = true;
      }
    } else {
      guard1 = true;
    }

    if (guard1) {
      ii++;
    }
  }

  if (1 > idx) {
    ii = 0;
  } else {
    ii = idx;
  }

  iInfite_size[0] = ii;
  for (i7 = 0; i7 < ii; i7++) {
    iInfite_data[i7] = ii_data[i7];
  }

  memcpy(&yTemp[0], &Yin[0], 200U * sizeof(double));
  idx = iInfite_size[0];
  ii = iInfite_size[0];
  for (i7 = 0; i7 < ii; i7++) {
    ii_data[i7] = (int)iInfite_data[i7];
  }

  for (i7 = 0; i7 < idx; i7++) {
    yTemp[ii_data[i7] - 1] = rtNaN;
  }

  emxInit_real_T1(&iPk, 1);
  emxInit_real_T1(&base, 1);
  b_findLocalMaxima(yTemp, iPk, base);
  if (!(iPk->size[0] == 0)) {
    ii = iPk->size[0] - 1;
    trueCount = 0;
    for (i = 0; i <= ii; i++) {
      if (Yin[(int)iPk->data[i] - 1] > rtMinusInf) {
        trueCount++;
      }
    }

    idx = 0;
    for (i = 0; i <= ii; i++) {
      if (Yin[(int)iPk->data[i] - 1] > rtMinusInf) {
        iPk->data[idx] = iPk->data[i];
        idx++;
      }
    }

    i7 = iPk->size[0];
    iPk->size[0] = trueCount;
    emxEnsureCapacity((emxArray__common *)iPk, i7, (int)sizeof(double));
  }

  ii = iPk->size[0];
  i7 = base->size[0];
  base->size[0] = ii;
  emxEnsureCapacity((emxArray__common *)base, i7, (int)sizeof(double));
  for (idx = 0; idx + 1 <= ii; idx++) {
    if ((Yin[(int)(iPk->data[idx] - 1.0) - 1] >= Yin[(int)(iPk->data[idx] + 1.0)
         - 1]) || rtIsNaN(Yin[(int)(iPk->data[idx] + 1.0) - 1])) {
      b_iPk = Yin[(int)(iPk->data[idx] - 1.0) - 1];
    } else {
      b_iPk = Yin[(int)(iPk->data[idx] + 1.0) - 1];
    }

    base->data[idx] = b_iPk;
  }

  ii = iPk->size[0] - 1;
  trueCount = 0;
  for (i = 0; i <= ii; i++) {
    if (Yin[(int)iPk->data[i] - 1] - base->data[i] >= 0.0) {
      trueCount++;
    }
  }

  idx = 0;
  for (i = 0; i <= ii; i++) {
    if (Yin[(int)iPk->data[i] - 1] - base->data[i] >= 0.0) {
      iPk->data[idx] = iPk->data[i];
      idx++;
    }
  }

  emxInit_real_T1(&c_iPk, 1);
  i7 = iPk->size[0];
  iPk->size[0] = trueCount;
  emxEnsureCapacity((emxArray__common *)iPk, i7, (int)sizeof(double));
  b_iInfite_data.data = (double *)&iInfite_data;
  b_iInfite_data.size = (int *)&iInfite_size;
  b_iInfite_data.allocatedSize = 200;
  b_iInfite_data.numDimensions = 1;
  b_iInfite_data.canFreeData = false;
  combinePeaks(iPk, &b_iInfite_data, c_iPk);
  if (c_iPk->size[0] == 0) {
    emxInit_real_T(&c_sortIdx, 2);
    for (i7 = 0; i7 < 2; i7++) {
      idx = c_sortIdx->size[0] * c_sortIdx->size[1];
      c_sortIdx->size[i7] = 1 - i7;
      emxEnsureCapacity((emxArray__common *)c_sortIdx, idx, (int)sizeof(double));
    }

    i7 = base->size[0];
    base->size[0] = c_sortIdx->size[1];
    emxEnsureCapacity((emxArray__common *)base, i7, (int)sizeof(double));
    ii = c_sortIdx->size[1];
    for (i7 = 0; i7 < ii; i7++) {
      base->data[i7] = c_sortIdx->data[c_sortIdx->size[0] * i7];
    }

    emxFree_real_T(&c_sortIdx);
  } else {
    i7 = iPk->size[0];
    iPk->size[0] = c_iPk->size[0];
    emxEnsureCapacity((emxArray__common *)iPk, i7, (int)sizeof(double));
    ii = c_iPk->size[0];
    for (i7 = 0; i7 < ii; i7++) {
      iPk->data[i7] = 1.0 + (double)((int)c_iPk->data[i7] - 1);
    }

    i7 = base->size[0];
    base->size[0] = c_iPk->size[0];
    emxEnsureCapacity((emxArray__common *)base, i7, (int)sizeof(double));
    ii = c_iPk->size[0];
    for (i7 = 0; i7 < ii; i7++) {
      base->data[i7] = Yin[(int)c_iPk->data[i7] - 1];
    }

    emxInit_real_T1(&c_sortIdx, 1);
    emxInit_int32_T(&iidx, 1);
    sort(base, iidx);
    i7 = c_sortIdx->size[0];
    c_sortIdx->size[0] = iidx->size[0];
    emxEnsureCapacity((emxArray__common *)c_sortIdx, i7, (int)sizeof(double));
    ii = iidx->size[0];
    for (i7 = 0; i7 < ii; i7++) {
      c_sortIdx->data[i7] = iidx->data[i7];
    }

    i7 = base->size[0];
    base->size[0] = c_sortIdx->size[0];
    emxEnsureCapacity((emxArray__common *)base, i7, (int)sizeof(double));
    ii = c_sortIdx->size[0];
    for (i7 = 0; i7 < ii; i7++) {
      base->data[i7] = iPk->data[(int)c_sortIdx->data[i7] - 1];
    }

    emxInit_boolean_T(&idelete, 1);
    unnamed_idx_0 = (unsigned int)c_sortIdx->size[0];
    i7 = idelete->size[0];
    idelete->size[0] = (int)unnamed_idx_0;
    emxEnsureCapacity((emxArray__common *)idelete, i7, (int)sizeof(boolean_T));
    ii = (int)unnamed_idx_0;
    for (i7 = 0; i7 < ii; i7++) {
      idelete->data[i7] = false;
    }

    i = 0;
    emxInit_boolean_T(&r4, 1);
    while (i <= c_sortIdx->size[0] - 1) {
      if (!idelete->data[i]) {
        b_iPk = iPk->data[(int)c_sortIdx->data[i] - 1] - 50.0;
        d_iPk = iPk->data[(int)c_sortIdx->data[i] - 1] + 50.0;
        i7 = r4->size[0];
        r4->size[0] = base->size[0];
        emxEnsureCapacity((emxArray__common *)r4, i7, (int)sizeof(boolean_T));
        ii = base->size[0];
        for (i7 = 0; i7 < ii; i7++) {
          r4->data[i7] = ((base->data[i7] >= b_iPk) && (base->data[i7] <= d_iPk));
        }

        i7 = idelete->size[0];
        emxEnsureCapacity((emxArray__common *)idelete, i7, (int)sizeof(boolean_T));
        ii = idelete->size[0];
        for (i7 = 0; i7 < ii; i7++) {
          idelete->data[i7] = (idelete->data[i7] || r4->data[i7]);
        }

        idelete->data[i] = false;
      }

      i++;
    }

    emxFree_boolean_T(&r4);
    ii = idelete->size[0] - 1;
    trueCount = 0;
    for (i = 0; i <= ii; i++) {
      if (!idelete->data[i]) {
        trueCount++;
      }
    }

    i7 = iidx->size[0];
    iidx->size[0] = trueCount;
    emxEnsureCapacity((emxArray__common *)iidx, i7, (int)sizeof(int));
    idx = 0;
    for (i = 0; i <= ii; i++) {
      if (!idelete->data[i]) {
        iidx->data[idx] = i + 1;
        idx++;
      }
    }

    emxFree_boolean_T(&idelete);
    i7 = base->size[0];
    base->size[0] = iidx->size[0];
    emxEnsureCapacity((emxArray__common *)base, i7, (int)sizeof(double));
    ii = iidx->size[0];
    for (i7 = 0; i7 < ii; i7++) {
      base->data[i7] = c_sortIdx->data[iidx->data[i7] - 1];
    }

    emxFree_int32_T(&iidx);
    emxFree_real_T(&c_sortIdx);
    c_sort(base);
  }

  if (base->size[0] > 200) {
    emxInit_real_T1(&b_base, 1);
    i7 = b_base->size[0];
    b_base->size[0] = 200;
    emxEnsureCapacity((emxArray__common *)b_base, i7, (int)sizeof(double));
    for (i7 = 0; i7 < 200; i7++) {
      b_base->data[i7] = base->data[i7];
    }

    i7 = base->size[0];
    base->size[0] = b_base->size[0];
    emxEnsureCapacity((emxArray__common *)base, i7, (int)sizeof(double));
    ii = b_base->size[0];
    for (i7 = 0; i7 < ii; i7++) {
      base->data[i7] = b_base->data[i7];
    }

    emxFree_real_T(&b_base);
  }

  i7 = iPk->size[0];
  iPk->size[0] = base->size[0];
  emxEnsureCapacity((emxArray__common *)iPk, i7, (int)sizeof(double));
  ii = base->size[0];
  for (i7 = 0; i7 < ii; i7++) {
    iPk->data[i7] = c_iPk->data[(int)base->data[i7] - 1];
  }

  emxFree_real_T(&base);
  emxFree_real_T(&c_iPk);
  i7 = Ypk->size[0] * Ypk->size[1];
  Ypk->size[0] = 1;
  Ypk->size[1] = iPk->size[0];
  emxEnsureCapacity((emxArray__common *)Ypk, i7, (int)sizeof(double));
  ii = iPk->size[0];
  for (i7 = 0; i7 < ii; i7++) {
    Ypk->data[Ypk->size[0] * i7] = Yin[(int)iPk->data[i7] - 1];
  }

  i7 = Xpk->size[0] * Xpk->size[1];
  Xpk->size[0] = 1;
  Xpk->size[1] = iPk->size[0];
  emxEnsureCapacity((emxArray__common *)Xpk, i7, (int)sizeof(double));
  ii = iPk->size[0];
  for (i7 = 0; i7 < ii; i7++) {
    Xpk->data[Xpk->size[0] * i7] = 1.0 + (double)((int)iPk->data[i7] - 1);
  }

  emxFree_real_T(&iPk);
}

void findpeaks(const emxArray_real_T *Yin, emxArray_real_T *Ypk, emxArray_real_T
               *Xpk)
{
  emxArray_real_T *y;
  emxArray_real_T *x;
  emxArray_boolean_T *idelete;
  boolean_T yIsRow;
  boolean_T xIsRow;
  double maxN;
  int i3;
  int ii;
  emxArray_int32_T *b_ii;
  int nx;
  int idx;
  boolean_T exitg1;
  boolean_T guard1 = false;
  emxArray_real_T *iInfite;
  emxArray_real_T *yTemp;
  emxArray_real_T *iPk;
  emxArray_real_T *locs;
  int i;
  double b_locs;
  emxArray_real_T *b_iPk;
  emxArray_real_T *b_idx;
  emxArray_real_T *b_y;
  unsigned int unnamed_idx_0;
  emxArray_boolean_T *r0;
  double c_locs;
  emxInit_real_T1(&y, 1);
  emxInit_real_T1(&x, 1);
  emxInit_boolean_T(&idelete, 1);
  parse_inputs(Yin, y, &yIsRow, x, &xIsRow, &maxN);
  i3 = idelete->size[0];
  idelete->size[0] = y->size[0];
  emxEnsureCapacity((emxArray__common *)idelete, i3, (int)sizeof(boolean_T));
  ii = y->size[0];
  for (i3 = 0; i3 < ii; i3++) {
    idelete->data[i3] = rtIsInf(y->data[i3]);
  }

  i3 = idelete->size[0];
  emxEnsureCapacity((emxArray__common *)idelete, i3, (int)sizeof(boolean_T));
  ii = idelete->size[0];
  for (i3 = 0; i3 < ii; i3++) {
    idelete->data[i3] = (idelete->data[i3] && (y->data[i3] > 0.0));
  }

  emxInit_int32_T(&b_ii, 1);
  nx = idelete->size[0];
  idx = 0;
  i3 = b_ii->size[0];
  b_ii->size[0] = idelete->size[0];
  emxEnsureCapacity((emxArray__common *)b_ii, i3, (int)sizeof(int));
  ii = 1;
  exitg1 = false;
  while ((!exitg1) && (ii <= nx)) {
    guard1 = false;
    if (idelete->data[ii - 1]) {
      idx++;
      b_ii->data[idx - 1] = ii;
      if (idx >= nx) {
        exitg1 = true;
      } else {
        guard1 = true;
      }
    } else {
      guard1 = true;
    }

    if (guard1) {
      ii++;
    }
  }

  if (idelete->size[0] == 1) {
    if (idx == 0) {
      i3 = b_ii->size[0];
      b_ii->size[0] = 0;
      emxEnsureCapacity((emxArray__common *)b_ii, i3, (int)sizeof(int));
    }
  } else {
    i3 = b_ii->size[0];
    if (1 > idx) {
      b_ii->size[0] = 0;
    } else {
      b_ii->size[0] = idx;
    }

    emxEnsureCapacity((emxArray__common *)b_ii, i3, (int)sizeof(int));
  }

  emxInit_real_T1(&iInfite, 1);
  i3 = iInfite->size[0];
  iInfite->size[0] = b_ii->size[0];
  emxEnsureCapacity((emxArray__common *)iInfite, i3, (int)sizeof(double));
  ii = b_ii->size[0];
  for (i3 = 0; i3 < ii; i3++) {
    iInfite->data[i3] = b_ii->data[i3];
  }

  emxInit_real_T1(&yTemp, 1);
  i3 = yTemp->size[0];
  yTemp->size[0] = y->size[0];
  emxEnsureCapacity((emxArray__common *)yTemp, i3, (int)sizeof(double));
  ii = y->size[0];
  for (i3 = 0; i3 < ii; i3++) {
    yTemp->data[i3] = y->data[i3];
  }

  i3 = b_ii->size[0];
  b_ii->size[0] = iInfite->size[0];
  emxEnsureCapacity((emxArray__common *)b_ii, i3, (int)sizeof(int));
  ii = iInfite->size[0];
  for (i3 = 0; i3 < ii; i3++) {
    b_ii->data[i3] = (int)iInfite->data[i3];
  }

  ii = b_ii->size[0];
  for (i3 = 0; i3 < ii; i3++) {
    yTemp->data[b_ii->data[i3] - 1] = rtNaN;
  }

  emxInit_real_T1(&iPk, 1);
  emxInit_real_T1(&locs, 1);
  findLocalMaxima(yTemp, iPk, locs);
  if (!(iPk->size[0] == 0)) {
    nx = iPk->size[0] - 1;
    idx = 0;
    for (i = 0; i <= nx; i++) {
      if (y->data[(int)iPk->data[i] - 1] > 50.0) {
        idx++;
      }
    }

    ii = 0;
    for (i = 0; i <= nx; i++) {
      if (y->data[(int)iPk->data[i] - 1] > 50.0) {
        iPk->data[ii] = iPk->data[i];
        ii++;
      }
    }

    i3 = iPk->size[0];
    iPk->size[0] = idx;
    emxEnsureCapacity((emxArray__common *)iPk, i3, (int)sizeof(double));
  }

  nx = iPk->size[0];
  i3 = yTemp->size[0];
  yTemp->size[0] = nx;
  emxEnsureCapacity((emxArray__common *)yTemp, i3, (int)sizeof(double));
  for (ii = 0; ii + 1 <= nx; ii++) {
    if ((y->data[(int)(iPk->data[ii] - 1.0) - 1] >= y->data[(int)(iPk->data[ii]
          + 1.0) - 1]) || rtIsNaN(y->data[(int)(iPk->data[ii] + 1.0) - 1])) {
      b_locs = y->data[(int)(iPk->data[ii] - 1.0) - 1];
    } else {
      b_locs = y->data[(int)(iPk->data[ii] + 1.0) - 1];
    }

    yTemp->data[ii] = b_locs;
  }

  nx = iPk->size[0] - 1;
  idx = 0;
  for (i = 0; i <= nx; i++) {
    if (y->data[(int)iPk->data[i] - 1] - yTemp->data[i] >= 0.0) {
      idx++;
    }
  }

  ii = 0;
  for (i = 0; i <= nx; i++) {
    if (y->data[(int)iPk->data[i] - 1] - yTemp->data[i] >= 0.0) {
      iPk->data[ii] = iPk->data[i];
      ii++;
    }
  }

  emxInit_real_T1(&b_iPk, 1);
  i3 = iPk->size[0];
  iPk->size[0] = idx;
  emxEnsureCapacity((emxArray__common *)iPk, i3, (int)sizeof(double));
  combinePeaks(iPk, iInfite, b_iPk);
  emxInit_real_T1(&b_idx, 1);
  if (b_iPk->size[0] == 0) {
    emxInit_real_T(&b_y, 2);
    for (i3 = 0; i3 < 2; i3++) {
      ii = b_y->size[0] * b_y->size[1];
      b_y->size[i3] = 1 - i3;
      emxEnsureCapacity((emxArray__common *)b_y, ii, (int)sizeof(double));
    }

    i3 = b_idx->size[0];
    b_idx->size[0] = b_y->size[1];
    emxEnsureCapacity((emxArray__common *)b_idx, i3, (int)sizeof(double));
    ii = b_y->size[1];
    for (i3 = 0; i3 < ii; i3++) {
      b_idx->data[i3] = b_y->data[b_y->size[0] * i3];
    }

    emxFree_real_T(&b_y);
  } else {
    i3 = locs->size[0];
    locs->size[0] = b_iPk->size[0];
    emxEnsureCapacity((emxArray__common *)locs, i3, (int)sizeof(double));
    ii = b_iPk->size[0];
    for (i3 = 0; i3 < ii; i3++) {
      locs->data[i3] = x->data[(int)b_iPk->data[i3] - 1];
    }

    i3 = yTemp->size[0];
    yTemp->size[0] = b_iPk->size[0];
    emxEnsureCapacity((emxArray__common *)yTemp, i3, (int)sizeof(double));
    ii = b_iPk->size[0];
    for (i3 = 0; i3 < ii; i3++) {
      yTemp->data[i3] = y->data[(int)b_iPk->data[i3] - 1];
    }

    sort(yTemp, b_ii);
    i3 = iInfite->size[0];
    iInfite->size[0] = b_ii->size[0];
    emxEnsureCapacity((emxArray__common *)iInfite, i3, (int)sizeof(double));
    ii = b_ii->size[0];
    for (i3 = 0; i3 < ii; i3++) {
      iInfite->data[i3] = b_ii->data[i3];
    }

    i3 = yTemp->size[0];
    yTemp->size[0] = iInfite->size[0];
    emxEnsureCapacity((emxArray__common *)yTemp, i3, (int)sizeof(double));
    ii = iInfite->size[0];
    for (i3 = 0; i3 < ii; i3++) {
      yTemp->data[i3] = locs->data[(int)iInfite->data[i3] - 1];
    }

    unnamed_idx_0 = (unsigned int)iInfite->size[0];
    i3 = idelete->size[0];
    idelete->size[0] = (int)unnamed_idx_0;
    emxEnsureCapacity((emxArray__common *)idelete, i3, (int)sizeof(boolean_T));
    ii = (int)unnamed_idx_0;
    for (i3 = 0; i3 < ii; i3++) {
      idelete->data[i3] = false;
    }

    i = 0;
    emxInit_boolean_T(&r0, 1);
    while (i <= iInfite->size[0] - 1) {
      if (!idelete->data[i]) {
        b_locs = locs->data[(int)iInfite->data[i] - 1] - 200.0;
        c_locs = locs->data[(int)iInfite->data[i] - 1] + 200.0;
        i3 = r0->size[0];
        r0->size[0] = yTemp->size[0];
        emxEnsureCapacity((emxArray__common *)r0, i3, (int)sizeof(boolean_T));
        ii = yTemp->size[0];
        for (i3 = 0; i3 < ii; i3++) {
          r0->data[i3] = ((yTemp->data[i3] >= b_locs) && (yTemp->data[i3] <=
            c_locs));
        }

        i3 = idelete->size[0];
        emxEnsureCapacity((emxArray__common *)idelete, i3, (int)sizeof(boolean_T));
        ii = idelete->size[0];
        for (i3 = 0; i3 < ii; i3++) {
          idelete->data[i3] = (idelete->data[i3] || r0->data[i3]);
        }

        idelete->data[i] = false;
      }

      i++;
    }

    emxFree_boolean_T(&r0);
    nx = idelete->size[0] - 1;
    idx = 0;
    for (i = 0; i <= nx; i++) {
      if (!idelete->data[i]) {
        idx++;
      }
    }

    i3 = b_ii->size[0];
    b_ii->size[0] = idx;
    emxEnsureCapacity((emxArray__common *)b_ii, i3, (int)sizeof(int));
    ii = 0;
    for (i = 0; i <= nx; i++) {
      if (!idelete->data[i]) {
        b_ii->data[ii] = i + 1;
        ii++;
      }
    }

    i3 = b_idx->size[0];
    b_idx->size[0] = b_ii->size[0];
    emxEnsureCapacity((emxArray__common *)b_idx, i3, (int)sizeof(double));
    ii = b_ii->size[0];
    for (i3 = 0; i3 < ii; i3++) {
      b_idx->data[i3] = iInfite->data[b_ii->data[i3] - 1];
    }

    c_sort(b_idx);
  }

  emxFree_boolean_T(&idelete);
  emxFree_int32_T(&b_ii);
  emxFree_real_T(&iInfite);
  if (b_idx->size[0] > maxN) {
    i3 = b_idx->size[0];
    if (1.0 > maxN) {
      b_idx->size[0] = 0;
    } else {
      b_idx->size[0] = (int)maxN;
    }

    emxEnsureCapacity((emxArray__common *)b_idx, i3, (int)sizeof(double));
  }

  i3 = iPk->size[0];
  iPk->size[0] = b_idx->size[0];
  emxEnsureCapacity((emxArray__common *)iPk, i3, (int)sizeof(double));
  ii = b_idx->size[0];
  for (i3 = 0; i3 < ii; i3++) {
    iPk->data[i3] = b_iPk->data[(int)b_idx->data[i3] - 1];
  }

  emxFree_real_T(&b_iPk);
  i3 = yTemp->size[0];
  yTemp->size[0] = iPk->size[0];
  emxEnsureCapacity((emxArray__common *)yTemp, i3, (int)sizeof(double));
  ii = iPk->size[0];
  for (i3 = 0; i3 < ii; i3++) {
    yTemp->data[i3] = y->data[(int)iPk->data[i3] - 1];
  }

  emxFree_real_T(&y);
  i3 = locs->size[0];
  locs->size[0] = iPk->size[0];
  emxEnsureCapacity((emxArray__common *)locs, i3, (int)sizeof(double));
  ii = iPk->size[0];
  for (i3 = 0; i3 < ii; i3++) {
    locs->data[i3] = x->data[(int)iPk->data[i3] - 1];
  }

  emxFree_real_T(&x);
  emxFree_real_T(&iPk);
  if (yIsRow) {
    i3 = Ypk->size[0] * Ypk->size[1];
    Ypk->size[0] = 1;
    Ypk->size[1] = yTemp->size[0];
    emxEnsureCapacity((emxArray__common *)Ypk, i3, (int)sizeof(double));
    ii = yTemp->size[0];
    for (i3 = 0; i3 < ii; i3++) {
      Ypk->data[Ypk->size[0] * i3] = yTemp->data[i3];
    }
  } else {
    i3 = Ypk->size[0] * Ypk->size[1];
    Ypk->size[0] = b_idx->size[0];
    Ypk->size[1] = 1;
    emxEnsureCapacity((emxArray__common *)Ypk, i3, (int)sizeof(double));
    ii = b_idx->size[0];
    for (i3 = 0; i3 < ii; i3++) {
      Ypk->data[i3] = yTemp->data[i3];
    }
  }

  emxFree_real_T(&yTemp);
  if (xIsRow) {
    i3 = Xpk->size[0] * Xpk->size[1];
    Xpk->size[0] = 1;
    Xpk->size[1] = locs->size[0];
    emxEnsureCapacity((emxArray__common *)Xpk, i3, (int)sizeof(double));
    ii = locs->size[0];
    for (i3 = 0; i3 < ii; i3++) {
      Xpk->data[Xpk->size[0] * i3] = locs->data[i3];
    }
  } else {
    i3 = Xpk->size[0] * Xpk->size[1];
    Xpk->size[0] = b_idx->size[0];
    Xpk->size[1] = 1;
    emxEnsureCapacity((emxArray__common *)Xpk, i3, (int)sizeof(double));
    ii = b_idx->size[0];
    for (i3 = 0; i3 < ii; i3++) {
      Xpk->data[i3] = locs->data[i3];
    }
  }

  emxFree_real_T(&locs);
  emxFree_real_T(&b_idx);
}

/* End of code generation (findpeaks.c) */
