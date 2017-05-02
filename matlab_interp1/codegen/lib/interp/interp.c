/*
 * File: interp.c
 *
 * MATLAB Coder version            : 2.6
 * C/C++ source code generated on  : 01-May-2017 17:55:34
 */

/* Include files */
#include "rt_nonfinite.h"
#include "interp.h"
#include "interp_emxutil.h"

/* Function Definitions */

/*
 * INTERP1 Summary of this function goes here
 *    Detailed explanation goes here
 * Arguments    : const emxArray_real_T *x
 *                const emxArray_real_T *y
 *                int sample_rate
 *                emxArray_real_T *p
 * Return Type  : void
 */
void interp(const emxArray_real_T *x, const emxArray_real_T *y, int sample_rate,
            emxArray_real_T *p)
{
  int nd2;
  emxArray_real_T *b_y;
  int low_ip1;
  double delta1;
  int k;
  emxArray_real_T *c_y;
  emxArray_real_T *b_x;
  unsigned int outsize[2];
  int32_T exitg1;
  int high_i;
  int mid_i;
  nd2 = sample_rate;
  if (sample_rate < 0) {
    nd2 = 0;
  }

  emxInit_real_T(&b_y, 2);
  low_ip1 = b_y->size[0] * b_y->size[1];
  b_y->size[0] = 1;
  b_y->size[1] = nd2;
  emxEnsureCapacity((emxArray__common *)b_y, low_ip1, (int)sizeof(double));
  if (nd2 >= 1) {
    b_y->data[nd2 - 1] = x->size[1];
    if (b_y->size[1] >= 2) {
      b_y->data[0] = 1.0;
      if (b_y->size[1] >= 3) {
        delta1 = ((double)x->size[1] - 1.0) / ((double)b_y->size[1] - 1.0);
        low_ip1 = b_y->size[1];
        for (k = 0; k <= low_ip1 - 3; k++) {
          b_y->data[k + 1] = 1.0 + (1.0 + (double)k) * delta1;
        }
      }
    }
  }

  emxInit_real_T(&c_y, 2);
  low_ip1 = c_y->size[0] * c_y->size[1];
  c_y->size[0] = 1;
  c_y->size[1] = y->size[1];
  emxEnsureCapacity((emxArray__common *)c_y, low_ip1, (int)sizeof(double));
  nd2 = y->size[0] * y->size[1];
  for (low_ip1 = 0; low_ip1 < nd2; low_ip1++) {
    c_y->data[low_ip1] = y->data[low_ip1];
  }

  emxInit_real_T(&b_x, 2);
  low_ip1 = b_x->size[0] * b_x->size[1];
  b_x->size[0] = 1;
  b_x->size[1] = x->size[1];
  emxEnsureCapacity((emxArray__common *)b_x, low_ip1, (int)sizeof(double));
  nd2 = x->size[0] * x->size[1];
  for (low_ip1 = 0; low_ip1 < nd2; low_ip1++) {
    b_x->data[low_ip1] = x->data[low_ip1];
  }

  for (low_ip1 = 0; low_ip1 < 2; low_ip1++) {
    outsize[low_ip1] = (unsigned int)b_y->size[low_ip1];
  }

  low_ip1 = p->size[0] * p->size[1];
  p->size[0] = 1;
  emxEnsureCapacity((emxArray__common *)p, low_ip1, (int)sizeof(double));
  low_ip1 = p->size[0] * p->size[1];
  p->size[1] = (int)outsize[1];
  emxEnsureCapacity((emxArray__common *)p, low_ip1, (int)sizeof(double));
  nd2 = (int)outsize[1];
  for (low_ip1 = 0; low_ip1 < nd2; low_ip1++) {
    p->data[low_ip1] = rtNaN;
  }

  if (b_y->size[1] == 0) {
  } else {
    k = 1;
    do {
      exitg1 = 0;
      if (k <= x->size[1]) {
        if (rtIsNaN(x->data[k - 1])) {
          exitg1 = 1;
        } else {
          k++;
        }
      } else {
        if (x->data[1] < x->data[0]) {
          low_ip1 = x->size[1] >> 1;
          for (high_i = 1; high_i <= low_ip1; high_i++) {
            delta1 = b_x->data[high_i - 1];
            b_x->data[high_i - 1] = b_x->data[x->size[1] - high_i];
            b_x->data[x->size[1] - high_i] = delta1;
          }

          low_ip1 = y->size[1];
          nd2 = (low_ip1 + (low_ip1 < 0)) >> 1;
          for (high_i = 1; high_i <= nd2; high_i++) {
            low_ip1 = y->size[1] - high_i;
            delta1 = c_y->data[c_y->size[0] * (high_i - 1)];
            c_y->data[c_y->size[0] * (high_i - 1)] = c_y->data[c_y->size[0] *
              low_ip1];
            c_y->data[c_y->size[0] * low_ip1] = delta1;
          }
        }

        for (k = 0; k + 1 <= b_y->size[1]; k++) {
          if ((b_y->data[k] > b_x->data[b_x->size[1] - 1]) || (b_y->data[k] <
               b_x->data[0])) {
          } else {
            nd2 = 1;
            low_ip1 = 2;
            high_i = b_x->size[1];
            while (high_i > low_ip1) {
              mid_i = (nd2 >> 1) + (high_i >> 1);
              if (((nd2 & 1) == 1) && ((high_i & 1) == 1)) {
                mid_i++;
              }

              if (b_y->data[k] >= b_x->data[mid_i - 1]) {
                nd2 = mid_i;
                low_ip1 = mid_i + 1;
              } else {
                high_i = mid_i;
              }
            }

            delta1 = (b_y->data[k] - b_x->data[nd2 - 1]) / (b_x->data[nd2] -
              b_x->data[nd2 - 1]);
            if (c_y->data[nd2 - 1] == c_y->data[nd2]) {
              p->data[k] = c_y->data[nd2 - 1];
            } else {
              p->data[k] = (1.0 - delta1) * c_y->data[nd2 - 1] + delta1 *
                c_y->data[nd2];
            }
          }
        }

        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }

  emxFree_real_T(&b_x);
  emxFree_real_T(&c_y);
  emxFree_real_T(&b_y);
}

/*
 * File trailer for interp.c
 *
 * [EOF]
 */
