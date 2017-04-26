//
//  statistics.c
//  classifyStrid
//
//  Created by Regan Hsu on 3/23/17.
//  Copyright © 2017 Regan Hsu. All rights reserved.
//

#include "statistics.h"

double w_mean(double *arr, int n)
{
    double total;
    int i;
    
    total = 0.0f;
    for (i = 0; i < n; i++) {
        total += arr[i];
    }
    return total/((float) n);
}

double w_mean_float(float *arr, int n)
{
    double total;
    int i;
    
    total = 0.0f;
    for (i = 0; i < n; i++) {
        total += arr[i];
    }
    return total/((float) n);
}

double w_std(double* data, int n)
{
    double sum = 0.0, mean, standardDeviation = 0.0;
    
    int i;
    
    for(i=0; i<n; ++i)
    {
        sum += data[i];
    }
    
    mean = sum/n;
    
    for(i=0; i<n; ++i)
        standardDeviation += pow(data[i] - mean, 2);
    
    return sqrt(standardDeviation/n);
}
float w_calculateSD(float* data, int n)
{
    float sum = 0.0, mean, standardDeviation = 0.0;
    
    int i;
    
    for(i=0; i<n; ++i)
    {
        sum += data[i];
    }
    
    mean = sum/n;
    
    for(i=0; i<n; ++i)
        standardDeviation += pow(data[i] - mean, 2);
    
    return sqrt(standardDeviation/n);
}

float w_minima(float* a, int n)
{
    int c, index;
    float min;
    min = a[0];
    index = 0;
    for (c = 1; c < n; c++) {
        if (a[c] < min) {
            index = c;
            min = a[c];
        }
    }
    return a[index];
}

float w_minima_seg(float* a, int begin, int end)
{
    int c, index;
    float min;
    min = a[begin];
    index = begin;
    for (c = index; c < end; c++) {
        if (a[c] < min) {
            index = c;
            min = a[c];
        }
    }
    return a[index];
}

double w_minima_double_seg(double *a, int begin, int end)
{
    int c, index;
    double min;
    min = a[begin];
    index = begin;
    for (c = index; c < end; c++) {
        if (a[c] < min) {
            index = c;
            min = a[c];
        }
    }
    return a[index];
}

float w_maxima(float* a, int n) {
    int c, index;
    float max;
    max = a[0];
    index = 0;
    for (c = 1; c < n; c++) {
        if (a[c] > max) {
            index = c;
            max = a[c];
        }
    }
    return a[index];
}

double w_maxima_double_seg(double *a, int begin, int end)
{
    int c, index;
    double max;
    max = a[begin];
    index = begin;
    for (c = index; c < end; c++) {
        if (a[c] > max) {
            index = c;
            max = a[c];
        }
    }
    return a[index];
}

float w_maxima_seg(float* a, int begin, int end)
{
    int c, index;
    float max;
    max = a[begin];
    index = begin;
    for (c = index; c < end; c++) {
        if (a[c] > max) {
            index = c;
            max = a[c];
        }
    }
    return a[index];
}

double w_RMS_seg (float arr[], int size)

{
    int i;
    double sumsq;
    double RMS;
    sumsq = 0;
    
    
    for (i = 0; i< size; i++)
    {
        sumsq += arr[i]*arr[i];
        
    }

    RMS = (1.0/size)*(sqrt(sumsq));
    return RMS;
}

double w_RMS_seg_double (double arr[], int size)

{
    int i;
    double sumsq;
    double RMS;
    sumsq = 0;
    
    
    for (i = 0; i< size; i++)
    {
        sumsq += arr[i]*arr[i];
        
    } 
    
    RMS = (1.0/size)*(sqrt(sumsq));
    return RMS;
}

int cmpfunc (const void *a, const void *b) {
    return ( *(float*) a - *(float*) b);
}
