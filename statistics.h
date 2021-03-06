//
//  statistics.h
//  classifyStrid
//
//  Created by Regan Hsu on 3/23/17.
//  Copyright © 2017 Regan Hsu. All rights reserved.
//

#ifndef statistics_h
#define statistics_h

#include <stdio.h>
/* for string manip */
#include <string.h>
/* for exit() */
#include <stdlib.h>
/* for fabsf() */
#include <math.h>

double w_mean_float(float *arr, int n);
double w_mean(double *arr, int n);
double w_std(double*, int);
float w_calculateSD(float data[], int n);

float w_walk_minima(float *a, int n);
float w_minima_seg(float *a, int begin, int end);
double w_minima_double_seg(double *a, int begin, int end);



float w_maxima(float *a, int n);
float w_maxima_seg(float *a, int begin, int end);
double w_maxima_double_seg(double *a, int begin, int end);
double w_RMS_seg_double (double arr[], int size);
double w_RMS_seg (float arr[], int size);

#endif /* statistics_h */

