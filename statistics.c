//
//  statistics.c
//  classifyStrid
//
//  Created by Regan Hsu on 3/23/17.
//  Copyright © 2017 Regan Hsu. All rights reserved.
//

#include "statistics.h"

double w_mean(const double *arr, int n)
{
    double total;
    int i;
    
    total = 0.0f;
    for (i = 0; i < n; i++) {
        total += arr[i];
    }
    return total/((float) n);
}

double w_mean_float(const float *arr, int n)
{
    double total;
    int i;
    
    total = 0.0f;
    for (i = 0; i < n; i++) {
        total += arr[i];
    }
    return total/((float) n);
}

double w_std(const double* data, int n)
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

double w_RMS_seg_double (const double  arr[], int size)

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




float calculate_mean(const float *segment, int n)
{
	int i;
    float sum = 0;
	
    for (i=0; i<n; i++) {
        sum += segment[i];
    }
    return sum/((float) n);
}


double calculate_mean_double(const double *segment, int n)
{
    int i;
    double sum = 0;
    
    for (i=0; i<n; i++) {
        sum += segment[i];
    }
    return sum/((double) n);
}


float calculate_standard_deviation(const float* segment, int n)
{
    float mean = 0;
	float sum = 0;
	float standard_deviation = 0;
    int i;
    
	mean = calculate_mean(segment, n);
    
    for(i=0; i<n; i++){
        sum += (float)pow((double)segment[i] - (double)mean, 2);
    }
	standard_deviation = sqrt(sum/n);
    return standard_deviation;
}

double calculate_standard_deviation_double(const double* segment, int n)
{
    double mean = 0;
    double sum = 0;
    double standard_deviation = 0;
    int i;
    
    mean = calculate_mean_double(segment, n);
    
    for(i=0; i<n; i++){
        sum += (double)pow((double)segment[i] - (double)mean, 2);
    }
    standard_deviation = sqrt(sum/n);
    return standard_deviation;
}

void swap(float *a, float *b){
	float c;

	c = *a;
	*a = *b;
	*b = c;	
}

float calculate_median(float *segment, int n){
	int i, j, k;
	float median = 0;
	
	for(i=0; i<n; i++){
		for(j=0; j<n-i;j++){
			if(segment[j]>segment[j+1])
				swap(&segment[j],&segment[j+1]);
		}
	}
	
	if (n/2 % 2 == 0){
		k = n/2 - 1;
		median = segment[k];
	}
	else {
		k = (n+1)/2 -1;
		median = segment[k];
	
	}
 	return median;
}
	
	
float calculate_moment_skew(float *segment, int n){
	float skew;
	int i;
	float k = 0;
	float mean_3 =0;
	float mean = calculate_mean(segment, n);
	float standard_deviation = calculate_standard_deviation(segment, n);
	
	for(i=0;i<n;i++){
		k = segment[i] - mean;
		mean_3 += (float)pow((double)k, 3.0);
	}
	
	skew = ((n*mean_3)/((n-1)*(n-2)* (float)pow((double)standard_deviation, 3.0)));
		
	return skew;
	
}

float calculate_kurtosis(float *segment, int n){
	float mean = calculate_mean(segment, n);
	float standard_deviation = calculate_standard_deviation(segment, n);
	int i;
	float mean_4 = 0;
	float kurtosis;
	float k = 0;
	
	for(i=0;i<n;i++){
		k = segment[i] - mean;
		mean_4 += (float)pow((double)k, 4.0);
	}
	
	kurtosis = mean_4/(n*(float)pow((double)standard_deviation, 4.0));
	return kurtosis; 
}

double calculate_kurtosis_d(const double *segment, int n)
{
double mean = w_mean(segment, n); 
double standard_deviation = w_std(segment, n); 
int i;
double mean_4 = 0.0; 
double kurtosis;
double k = 0; 
for(i=0;i<n;i++){
		k = segment[i] - mean;
		mean_4 += pow(k, 4.0);
	}
 kurtosis = mean_4/(n*pow(standard_deviation, 4.0)); 
 return kurtosis; 
	
}

