/*
 * segment.c
 *
 * Code generation for function 'segment'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "get_feature.h"
#include "segment.h"
#include "get_feature_emxutil.h"
#include "mean.h"
#include "findpeaks.h"

/* Function Definitions */
void segment(emxArray_real_T *m, emxArray_real_T *pos, emxArray_real_T *r)
{
  emxArray_real_T *b_m;
  int nm1d2;
  int i2;
  emxArray_real_T *p_val;
  emxArray_real_T *p_pos;
  int nx;
  emxArray_real_T *c_m;
  emxArray_real_T *d_m;
  double ndbl;
  int u1;
  int i;
  emxArray_real_T *downs;
  emxArray_real_T *y;
  emxArray_real_T *b_y;
  emxArray_real_T *varargin_2;
  double len;
  int mid_i;
  double anew;
  double apnd;
  double cdiff;
  int high_i;
  double c_y;
  int k;
  double absb;
  unsigned int outsize[2];
  int exitg1;
  emxInit_real_T1(&b_m, 1);

  /* fprintf('segment Inside\n'); */
  /* fprintf('segment Inside - zgyro\n'); */
  nm1d2 = m->size[0];
  i2 = b_m->size[0];
  b_m->size[0] = nm1d2;
  emxEnsureCapacity((emxArray__common *)b_m, i2, (int)sizeof(double));
  for (i2 = 0; i2 < nm1d2; i2++) {
    b_m->data[i2] = m->data[i2 + m->size[0] * 6];
  }

  emxInit_real_T(&p_val, 2);
  emxInit_real_T(&p_pos, 2);
  findpeaks(b_m, p_val, p_pos);
  emxFree_real_T(&b_m);
  i2 = pos->size[0];
  pos->size[0] = p_pos->size[0];
  emxEnsureCapacity((emxArray__common *)pos, i2, (int)sizeof(double));
  nm1d2 = p_pos->size[0];
  for (i2 = 0; i2 < nm1d2; i2++) {
    pos->data[i2] = p_pos->data[i2] + 1.0;
  }

  if ((p_val->size[0] == 0) || (p_val->size[1] == 0)) {
    nx = -1;
  } else if (p_val->size[0] > p_val->size[1]) {
    nx = p_val->size[0] - 1;
  } else {
    nx = p_val->size[1] - 1;
  }

  emxFree_real_T(&p_val);
  i2 = r->size[0] * r->size[1];
  r->size[0] = nx;
  r->size[1] = 200;
  emxEnsureCapacity((emxArray__common *)r, i2, (int)sizeof(double));
  nm1d2 = nx * 200;
  for (i2 = 0; i2 < nm1d2; i2++) {
    r->data[i2] = 0.0;
  }

  emxInit_real_T1(&c_m, 1);

  /* fprintf('segment Inside - findpeaksfinish\n'); */
  /* fprintf('%f\n',p_val(1)); */
  /* fprintf('segment Inside - findpeaksfinish2\n'); */
  /* fprintf('%f\n',p_pos(1)); */
  /* fprintf('segment Inside - findpeaksfinish3\n'); */
  /* fprintf('segment Inside - findpeaksfinish4\n'); */
  nm1d2 = m->size[0];
  i2 = c_m->size[0];
  c_m->size[0] = nm1d2;
  emxEnsureCapacity((emxArray__common *)c_m, i2, (int)sizeof(double));
  for (i2 = 0; i2 < nm1d2; i2++) {
    c_m->data[i2] = m->data[i2 + m->size[0] * 6];
  }

  emxInit_real_T1(&d_m, 1);
  nm1d2 = m->size[0];
  ndbl = mean(c_m);
  i2 = d_m->size[0];
  d_m->size[0] = nm1d2;
  emxEnsureCapacity((emxArray__common *)d_m, i2, (int)sizeof(double));
  emxFree_real_T(&c_m);
  for (i2 = 0; i2 < nm1d2; i2++) {
    d_m->data[i2] = m->data[i2 + m->size[0] * 6] - ndbl;
  }

  nm1d2 = d_m->size[0];
  for (i2 = 0; i2 < nm1d2; i2++) {
    m->data[i2 + m->size[0] * 6] = d_m->data[i2];
  }

  emxFree_real_T(&d_m);

  /* fprintf('segment Inside - demean\n'); */
  if ((p_pos->size[0] == 0) || (p_pos->size[1] == 0)) {
    u1 = 0;
  } else {
    nm1d2 = p_pos->size[0];
    u1 = p_pos->size[1];
    if (nm1d2 >= u1) {
      u1 = nm1d2;
    }
  }

  i = 0;
  emxInit_real_T(&downs, 2);
  emxInit_real_T(&y, 2);
  emxInit_real_T(&b_y, 2);
  emxInit_real_T1(&varargin_2, 1);
  while (i <= u1 - 2) {
    len = p_pos->data[1 + i] - p_pos->data[i];
    if (p_pos->data[i] > p_pos->data[1 + i] - 1.0) {
      i2 = 0;
      mid_i = 0;
    } else {
      i2 = (int)p_pos->data[i] - 1;
      mid_i = (int)(p_pos->data[1 + i] - 1.0);
    }

    if (rtIsNaN(len)) {
      nx = 1;
      anew = rtNaN;
      apnd = len;
    } else if (len < 1.0) {
      nx = 0;
      anew = 1.0;
      apnd = len;
    } else if (rtIsInf(len)) {
      nx = 1;
      anew = rtNaN;
      apnd = len;
    } else {
      anew = 1.0;
      ndbl = floor((len - 1.0) + 0.5);
      apnd = 1.0 + ndbl;
      cdiff = (1.0 + ndbl) - len;
      if (fabs(cdiff) < 4.4408920985006262E-16 * len) {
        ndbl++;
        apnd = len;
      } else if (cdiff > 0.0) {
        apnd = 1.0 + (ndbl - 1.0);
      } else {
        ndbl++;
      }

      if (ndbl >= 0.0) {
        nx = (int)ndbl;
      } else {
        nx = 0;
      }
    }

    high_i = y->size[0] * y->size[1];
    y->size[0] = 1;
    y->size[1] = nx;
    emxEnsureCapacity((emxArray__common *)y, high_i, (int)sizeof(double));
    if (nx > 0) {
      y->data[0] = anew;
      if (nx > 1) {
        y->data[nx - 1] = apnd;
        high_i = nx - 1;
        nm1d2 = high_i >> 1;
        for (k = 1; k < nm1d2; k++) {
          y->data[k] = anew + (double)k;
          y->data[(nx - k) - 1] = apnd - (double)k;
        }

        if (nm1d2 << 1 == nx - 1) {
          y->data[nm1d2] = (anew + apnd) / 2.0;
        } else {
          y->data[nm1d2] = anew + (double)nm1d2;
          y->data[nm1d2 + 1] = apnd - (double)nm1d2;
        }
      }
    }

    c_y = len / 200.0;
    if (rtIsNaN(c_y) || rtIsNaN(len)) {
      nx = 1;
      anew = rtNaN;
      apnd = len;
    } else if ((c_y == 0.0) || ((1.0 < len) && (c_y < 0.0)) || ((len < 1.0) &&
                (c_y > 0.0))) {
      nx = 0;
      anew = 1.0;
      apnd = len;
    } else if (rtIsInf(len)) {
      nx = 1;
      anew = rtNaN;
      apnd = len;
    } else if (rtIsInf(c_y)) {
      nx = 1;
      anew = 1.0;
      apnd = len;
    } else {
      anew = 1.0;
      ndbl = floor((len - 1.0) / c_y + 0.5);
      apnd = 1.0 + ndbl * c_y;
      if (c_y > 0.0) {
        cdiff = apnd - len;
      } else {
        cdiff = len - apnd;
      }

      absb = fabs(len);
      if ((1.0 >= absb) || rtIsNaN(absb)) {
        absb = 1.0;
      }

      if (fabs(cdiff) < 4.4408920985006262E-16 * absb) {
        ndbl++;
        apnd = len;
      } else if (cdiff > 0.0) {
        apnd = 1.0 + (ndbl - 1.0) * c_y;
      } else {
        ndbl++;
      }

      if (ndbl >= 0.0) {
        nx = (int)ndbl;
      } else {
        nx = 0;
      }
    }

    high_i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = nx;
    emxEnsureCapacity((emxArray__common *)b_y, high_i, (int)sizeof(double));
    if (nx > 0) {
      b_y->data[0] = anew;
      if (nx > 1) {
        b_y->data[nx - 1] = apnd;
        high_i = nx - 1;
        nm1d2 = high_i >> 1;
        for (k = 1; k < nm1d2; k++) {
          ndbl = (double)k * c_y;
          b_y->data[k] = anew + ndbl;
          b_y->data[(nx - k) - 1] = apnd - ndbl;
        }

        if (nm1d2 << 1 == nx - 1) {
          b_y->data[nm1d2] = (anew + apnd) / 2.0;
        } else {
          ndbl = (double)nm1d2 * c_y;
          b_y->data[nm1d2] = anew + ndbl;
          b_y->data[nm1d2 + 1] = apnd - ndbl;
        }
      }
    }

    high_i = varargin_2->size[0];
    varargin_2->size[0] = mid_i - i2;
    emxEnsureCapacity((emxArray__common *)varargin_2, high_i, (int)sizeof(double));
    nm1d2 = mid_i - i2;
    for (high_i = 0; high_i < nm1d2; high_i++) {
      varargin_2->data[high_i] = m->data[(i2 + high_i) + m->size[0] * 6];
    }

    nx = y->size[1];
    for (high_i = 0; high_i < 2; high_i++) {
      outsize[high_i] = (unsigned int)b_y->size[high_i];
    }

    high_i = downs->size[0] * downs->size[1];
    downs->size[0] = 1;
    downs->size[1] = (int)outsize[1];
    emxEnsureCapacity((emxArray__common *)downs, high_i, (int)sizeof(double));
    nm1d2 = (int)outsize[1];
    for (high_i = 0; high_i < nm1d2; high_i++) {
      downs->data[high_i] = rtNaN;
    }

    if (b_y->size[1] == 0) {
    } else {
      k = 1;
      do {
        exitg1 = 0;
        if (k <= nx) {
          if (rtIsNaN(y->data[k - 1])) {
            exitg1 = 1;
          } else {
            k++;
          }
        } else {
          if (y->data[1] < y->data[0]) {
            high_i = nx >> 1;
            for (nm1d2 = 1; nm1d2 <= high_i; nm1d2++) {
              ndbl = y->data[nm1d2 - 1];
              y->data[nm1d2 - 1] = y->data[nx - nm1d2];
              y->data[nx - nm1d2] = ndbl;
            }

            if ((!(mid_i - i2 == 0)) && (mid_i - i2 > 1)) {
              nx = mid_i - i2;
              nm1d2 = (mid_i - i2) >> 1;
              for (k = 1; k <= nm1d2; k++) {
                ndbl = varargin_2->data[k - 1];
                varargin_2->data[k - 1] = varargin_2->data[nx - k];
                varargin_2->data[nx - k] = ndbl;
              }
            }
          }

          for (k = 0; k + 1 <= b_y->size[1]; k++) {
            ndbl = downs->data[k];
            if (rtIsNaN(b_y->data[k])) {
              ndbl = rtNaN;
            } else if ((b_y->data[k] > y->data[y->size[1] - 1]) || (b_y->data[k]
                        < y->data[0])) {
            } else {
              nm1d2 = 1;
              nx = 2;
              high_i = y->size[1];
              while (high_i > nx) {
                mid_i = (nm1d2 >> 1) + (high_i >> 1);
                if (((nm1d2 & 1) == 1) && ((high_i & 1) == 1)) {
                  mid_i++;
                }

                if (b_y->data[k] >= y->data[mid_i - 1]) {
                  nm1d2 = mid_i;
                  nx = mid_i + 1;
                } else {
                  high_i = mid_i;
                }
              }

              ndbl = (b_y->data[k] - y->data[nm1d2 - 1]) / (y->data[nm1d2] -
                y->data[nm1d2 - 1]);
              if (ndbl == 0.0) {
                ndbl = varargin_2->data[nm1d2 - 1];
              } else if (ndbl == 1.0) {
                ndbl = varargin_2->data[nm1d2];
              } else if (varargin_2->data[nm1d2 - 1] == varargin_2->data[nm1d2])
              {
                ndbl = varargin_2->data[nm1d2 - 1];
              } else {
                ndbl = (1.0 - ndbl) * varargin_2->data[nm1d2 - 1] + ndbl *
                  varargin_2->data[nm1d2];
              }
            }

            downs->data[k] = ndbl;
          }

          exitg1 = 1;
        }
      } while (exitg1 == 0);
    }

    for (i2 = 0; i2 < 200; i2++) {
      r->data[i + r->size[0] * i2] = downs->data[i2];
    }

    i++;
  }

  emxFree_real_T(&varargin_2);
  emxFree_real_T(&b_y);
  emxFree_real_T(&y);
  emxFree_real_T(&p_pos);
  emxFree_real_T(&downs);

  /* fprintf('segment Inside - read return\n'); */
  /* hold off */
}

/* End of code generation (segment.c) */
