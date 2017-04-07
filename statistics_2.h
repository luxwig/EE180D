#ifndef statistics_2_h
#define statistics_2_h

#include <stdio.h>
#include <math.h>
#include <stdlib.h>


float swap(float *a, float *b);
float calculate_mean(float *segment, int n);
float calculate_median(float *segment, int n);
float calculate_standard_deviation(float *segment, int n);

float calculate_moment_skew(float *segment, int n);

float calculate_kurtosis(float *segment, int n);

#endif