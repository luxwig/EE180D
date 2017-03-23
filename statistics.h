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

float w_mean(float *arr, int n);
float w_calculateSD(float data[], int n);

float w_walk_minima(float *a, int n);
float w_minima_seg(float *a, int begin, int end);

float w_maxima(float *a, int n);
float w_maxima_seg(float *a, int begin, int end);

#endif /* statistics_h */

