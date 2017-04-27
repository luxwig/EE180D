/*
 * zanalysis.c
 *
 * Code generation for function 'zanalysis'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "get_feature.h"
#include "zanalysis.h"
#include "get_feature_emxutil.h"
#include "sort1.h"
#include "findpeaks.h"

/* Function Definitions */
void zanalysis(const emxArray_real_T *z_a, emxArray_real_T *val, emxArray_real_T
               *pos)
{
  int i5;
  int i;
  emxArray_real_T *p_pos;
  emxArray_real_T *qs;
  emxArray_real_T *tp_pos;
  emxArray_boolean_T *x;
  emxArray_int32_T *ii;
  emxArray_int32_T *b_ii;
  emxArray_real_T *b_pos;
  emxArray_real_T *b_p_pos;
  emxArray_real_T *c_p_pos;
  emxArray_real_T *d_p_pos;
  emxArray_real_T *e_p_pos;
  emxArray_real_T *f_p_pos;
  emxArray_real_T *b_z_a;
  emxArray_real_T *b_val;
  double c_z_a[200];
  int c_ii;
  boolean_T empty_non_axis_sizes;
  int loop_ub;
  int nx;
  int idx;
  double b_qs;
  int i6;
  boolean_T exitg2;
  boolean_T guard2 = false;
  boolean_T guard1 = false;
  boolean_T exitg1;
  boolean_T b_guard1 = false;
  i5 = pos->size[0] * pos->size[1];
  pos->size[0] = 0;
  pos->size[1] = 0;
  emxEnsureCapacity((emxArray__common *)pos, i5, (int)sizeof(double));
  i5 = val->size[0] * val->size[1];
  val->size[0] = 0;
  val->size[1] = 0;
  emxEnsureCapacity((emxArray__common *)val, i5, (int)sizeof(double));
  i = 0;
  emxInit_real_T(&p_pos, 2);
  emxInit_real_T(&qs, 2);
  emxInit_real_T(&tp_pos, 2);
  emxInit_boolean_T1(&x, 2);
  emxInit_int32_T1(&ii, 2);
  emxInit_int32_T1(&b_ii, 2);
  emxInit_real_T(&b_pos, 2);
  emxInit_real_T1(&b_p_pos, 1);
  emxInit_real_T1(&c_p_pos, 1);
  emxInit_real_T1(&d_p_pos, 1);
  emxInit_real_T1(&e_p_pos, 1);
  emxInit_real_T1(&f_p_pos, 1);
  emxInit_real_T(&b_z_a, 2);
  emxInit_real_T(&b_val, 2);
  while (i <= z_a->size[0] - 1) {
    for (i5 = 0; i5 < 200; i5++) {
      c_z_a[i5] = -z_a->data[i + z_a->size[0] * i5];
    }

    b_findpeaks(50, c_z_a, tp_pos, p_pos);
    if (tp_pos->size[1] < 2) 
    {
        i++;
        continue;
    }
    if (tp_pos->size[1] >= 2) {
      i5 = qs->size[0] * qs->size[1];
      qs->size[0] = 1;
      qs->size[1] = tp_pos->size[1];
      emxEnsureCapacity((emxArray__common *)qs, i5, (int)sizeof(double));
      loop_ub = tp_pos->size[0] * tp_pos->size[1];
      for (i5 = 0; i5 < loop_ub; i5++) {
        qs->data[i5] = tp_pos->data[i5];
      }

      d_sort(qs);
      i5 = x->size[0] * x->size[1];
      x->size[0] = 1;
      x->size[1] = tp_pos->size[1];
      emxEnsureCapacity((emxArray__common *)x, i5, (int)sizeof(boolean_T));
      b_qs = qs->data[0];
      loop_ub = tp_pos->size[0] * tp_pos->size[1];
      for (i5 = 0; i5 < loop_ub; i5++) {
        x->data[i5] = (tp_pos->data[i5] == b_qs);
      }

      nx = x->size[1];
      idx = 0;
      i5 = ii->size[0] * ii->size[1];
      ii->size[0] = 1;
      ii->size[1] = x->size[1];
      emxEnsureCapacity((emxArray__common *)ii, i5, (int)sizeof(int));
      c_ii = 1;
      exitg2 = false;
      while ((!exitg2) && (c_ii <= nx)) {
        guard2 = false;
        if (x->data[c_ii - 1]) {
          idx++;
          ii->data[idx - 1] = c_ii;
          if (idx >= nx) {
            exitg2 = true;
          } else {
            guard2 = true;
          }
        } else {
          guard2 = true;
        }

        if (guard2) {
          c_ii++;
        }
      }

      i5 = ii->size[0] * ii->size[1];
      if (1 > idx) {
        ii->size[1] = 0;
      } else {
        ii->size[1] = idx;
      }

      emxEnsureCapacity((emxArray__common *)ii, i5, (int)sizeof(int));
      i5 = x->size[0] * x->size[1];
      x->size[0] = 1;
      x->size[1] = tp_pos->size[1];
      emxEnsureCapacity((emxArray__common *)x, i5, (int)sizeof(boolean_T));
      b_qs = qs->data[1];
      loop_ub = tp_pos->size[0] * tp_pos->size[1];
      for (i5 = 0; i5 < loop_ub; i5++) {
        x->data[i5] = (tp_pos->data[i5] == b_qs);
      }

      nx = x->size[1];
      idx = 0;
      i5 = b_ii->size[0] * b_ii->size[1];
      b_ii->size[0] = 1;
      b_ii->size[1] = x->size[1];
      emxEnsureCapacity((emxArray__common *)b_ii, i5, (int)sizeof(int));
      c_ii = 1;
      exitg1 = false;
      while ((!exitg1) && (c_ii <= nx)) {
        b_guard1 = false;
        if (x->data[c_ii - 1]) {
          idx++;
          b_ii->data[idx - 1] = c_ii;
          if (idx >= nx) {
            exitg1 = true;
          } else {
            b_guard1 = true;
          }
        } else {
          b_guard1 = true;
        }

        if (b_guard1) {
          c_ii++;
        }
      }

      i5 = b_ii->size[0] * b_ii->size[1];
      if (1 > idx) {
        b_ii->size[1] = 0;
      } else {
        b_ii->size[1] = idx;
      }

      emxEnsureCapacity((emxArray__common *)b_ii, i5, (int)sizeof(int));
      i5 = tp_pos->size[0] * tp_pos->size[1];
      tp_pos->size[0] = 1;
      tp_pos->size[1] = ii->size[1] + b_ii->size[1];
      emxEnsureCapacity((emxArray__common *)tp_pos, i5, (int)sizeof(double));
      loop_ub = ii->size[1];
      for (i5 = 0; i5 < loop_ub; i5++) {
        tp_pos->data[tp_pos->size[0] * i5] = p_pos->data[ii->data[ii->size[0] *
          i5] - 1];
      }

      loop_ub = b_ii->size[1];
      for (i5 = 0; i5 < loop_ub; i5++) {
        tp_pos->data[tp_pos->size[0] * (i5 + ii->size[1])] = p_pos->data
          [b_ii->data[b_ii->size[0] * i5] - 1];
      }

      i5 = p_pos->size[0] * p_pos->size[1];
      p_pos->size[0] = 1;
      p_pos->size[1] = tp_pos->size[1];
      emxEnsureCapacity((emxArray__common *)p_pos, i5, (int)sizeof(double));
      loop_ub = tp_pos->size[0] * tp_pos->size[1];
      for (i5 = 0; i5 < loop_ub; i5++) {
        p_pos->data[i5] = tp_pos->data[i5];
      }

      f_sort(p_pos);
    }

    if (!((pos->size[0] == 0) || (pos->size[1] == 0))) {
      c_ii = pos->size[1];
    } else if (!(p_pos->size[1] == 0)) {
      c_ii = p_pos->size[1];
    } else {
      c_ii = pos->size[1];
      if (c_ii >= 0) {
      } else {
        c_ii = 0;
      }
    }

    empty_non_axis_sizes = (c_ii == 0);
    if (empty_non_axis_sizes || (!((pos->size[0] == 0) || (pos->size[1] == 0))))
    {
      nx = pos->size[0];
    } else {
      nx = 0;
    }

    if (empty_non_axis_sizes || (!(p_pos->size[1] == 0))) {
      idx = 1;
    } else {
      idx = 0;
    }

    i5 = b_pos->size[0] * b_pos->size[1];
    b_pos->size[0] = nx + idx;
    b_pos->size[1] = c_ii;
    emxEnsureCapacity((emxArray__common *)b_pos, i5, (int)sizeof(double));
    for (i5 = 0; i5 < c_ii; i5++) {
      for (i6 = 0; i6 < nx; i6++) {
        b_pos->data[i6 + b_pos->size[0] * i5] = pos->data[i6 + nx * i5];
      }
    }

    for (i5 = 0; i5 < c_ii; i5++) {
      for (i6 = 0; i6 < idx; i6++) {
        b_pos->data[(i6 + nx) + b_pos->size[0] * i5] = p_pos->data[i6 + idx * i5];
      }
    }

    i5 = pos->size[0] * pos->size[1];
    pos->size[0] = b_pos->size[0];
    pos->size[1] = b_pos->size[1];
    emxEnsureCapacity((emxArray__common *)pos, i5, (int)sizeof(double));
    loop_ub = b_pos->size[1];
    for (i5 = 0; i5 < loop_ub; i5++) {
      c_ii = b_pos->size[0];
      for (i6 = 0; i6 < c_ii; i6++) {
        pos->data[i6 + pos->size[0] * i5] = b_pos->data[i6 + b_pos->size[0] * i5];
      }
    }

    if (!((val->size[0] == 0) || (val->size[1] == 0))) {
      c_ii = val->size[1];
    } else {
      i5 = b_p_pos->size[0];
      b_p_pos->size[0] = p_pos->size[1];
      emxEnsureCapacity((emxArray__common *)b_p_pos, i5, (int)sizeof(double));
      loop_ub = p_pos->size[1];
      for (i5 = 0; i5 < loop_ub; i5++) {
        b_p_pos->data[i5] = p_pos->data[p_pos->size[0] * i5];
      }

      if (!(b_p_pos->size[0] == 0)) {
        i5 = e_p_pos->size[0];
        e_p_pos->size[0] = p_pos->size[1];
        emxEnsureCapacity((emxArray__common *)e_p_pos, i5, (int)sizeof(double));
        loop_ub = p_pos->size[1];
        for (i5 = 0; i5 < loop_ub; i5++) {
          e_p_pos->data[i5] = p_pos->data[p_pos->size[0] * i5];
        }

        c_ii = e_p_pos->size[0];
      } else {
        c_ii = val->size[1];
        if (c_ii >= 0) {
        } else {
          c_ii = 0;
        }

        i5 = c_p_pos->size[0];
        c_p_pos->size[0] = p_pos->size[1];
        emxEnsureCapacity((emxArray__common *)c_p_pos, i5, (int)sizeof(double));
        loop_ub = p_pos->size[1];
        for (i5 = 0; i5 < loop_ub; i5++) {
          c_p_pos->data[i5] = p_pos->data[p_pos->size[0] * i5];
        }

        if (c_p_pos->size[0] > c_ii) {
          i5 = d_p_pos->size[0];
          d_p_pos->size[0] = p_pos->size[1];
          emxEnsureCapacity((emxArray__common *)d_p_pos, i5, (int)sizeof(double));
          loop_ub = p_pos->size[1];
          for (i5 = 0; i5 < loop_ub; i5++) {
            d_p_pos->data[i5] = p_pos->data[p_pos->size[0] * i5];
          }

          c_ii = d_p_pos->size[0];
        }
      }
    }

    empty_non_axis_sizes = (c_ii == 0);
    if (empty_non_axis_sizes || (!((val->size[0] == 0) || (val->size[1] == 0))))
    {
      nx = val->size[0];
    } else {
      nx = 0;
    }

    guard1 = false;
    if (empty_non_axis_sizes) {
      guard1 = true;
    } else {
      i5 = f_p_pos->size[0];
      f_p_pos->size[0] = p_pos->size[1];
      emxEnsureCapacity((emxArray__common *)f_p_pos, i5, (int)sizeof(double));
      loop_ub = p_pos->size[1];
      for (i5 = 0; i5 < loop_ub; i5++) {
        f_p_pos->data[i5] = p_pos->data[p_pos->size[0] * i5];
      }

      if (!(f_p_pos->size[0] == 0)) {
        guard1 = true;
      } else {
        idx = 0;
      }
    }

    if (guard1) {
      idx = 1;
    }

    i5 = b_z_a->size[0] * b_z_a->size[1];
    b_z_a->size[0] = 1;
    b_z_a->size[1] = p_pos->size[1];
    emxEnsureCapacity((emxArray__common *)b_z_a, i5, (int)sizeof(double));
    loop_ub = p_pos->size[1];
    for (i5 = 0; i5 < loop_ub; i5++) {
      b_z_a->data[b_z_a->size[0] * i5] = z_a->data[i + z_a->size[0] * ((int)
        p_pos->data[p_pos->size[0] * i5] - 1)];
    }

    i5 = b_val->size[0] * b_val->size[1];
    b_val->size[0] = nx + idx;
    b_val->size[1] = c_ii;
    emxEnsureCapacity((emxArray__common *)b_val, i5, (int)sizeof(double));
    for (i5 = 0; i5 < c_ii; i5++) {
      for (i6 = 0; i6 < nx; i6++) {
        b_val->data[i6 + b_val->size[0] * i5] = val->data[i6 + nx * i5];
      }
    }

    for (i5 = 0; i5 < c_ii; i5++) {
      for (i6 = 0; i6 < idx; i6++) {
        b_val->data[(i6 + nx) + b_val->size[0] * i5] = b_z_a->data[i6 + idx * i5];
      }
    }

    i5 = val->size[0] * val->size[1];
    val->size[0] = b_val->size[0];
    val->size[1] = b_val->size[1];
    emxEnsureCapacity((emxArray__common *)val, i5, (int)sizeof(double));
    loop_ub = b_val->size[1];
    for (i5 = 0; i5 < loop_ub; i5++) {
      c_ii = b_val->size[0];
      for (i6 = 0; i6 < c_ii; i6++) {
        val->data[i6 + val->size[0] * i5] = b_val->data[i6 + b_val->size[0] * i5];
      }
    }

    i++;
  }

  emxFree_real_T(&b_val);
  emxFree_real_T(&b_z_a);
  emxFree_real_T(&f_p_pos);
  emxFree_real_T(&e_p_pos);
  emxFree_real_T(&d_p_pos);
  emxFree_real_T(&c_p_pos);
  emxFree_real_T(&b_p_pos);
  emxFree_real_T(&b_pos);
  emxFree_int32_T(&b_ii);
  emxFree_int32_T(&ii);
  emxFree_boolean_T(&x);
  emxFree_real_T(&tp_pos);
  emxFree_real_T(&qs);
  emxFree_real_T(&p_pos);
}

/* End of code generation (zanalysis.c) */
