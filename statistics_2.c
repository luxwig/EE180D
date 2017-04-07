#include "statistics_2.h"


float calculate_mean(float *segment, int n)
{
	int i;
    float sum;
	
    for (i=0; i<n; i++) {
        sum += arr[i];
    }
    return sum/((float) n);
}
float calculate_standard_deviation(float* segment, int n)
{
    float mean;
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

float swap(float *a, float *b){
	float c;

	c = *a;
	*a = *b;
	*b = c;	
}

float calculate_median(float *segment, int n){
	int i, j, k;
	float median;
	
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
	return median;
	
	}
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
	
	skew = ((n*mean_3)/((n-1)*(n-2)* (float)pow((double)standard_deviation, 3.0));
		
	return skew;
	
}

float calculate_kurtosis(float *segment, int n){
	float mean = calculate_mean(segment, n);
	float standard_deviation = calculate_standard_deviation(segment, n);
	int i;
	float mean_4, kurtosis;
	float k = 0;
	
	for(i=0;i<n;i++){
		k = segment[i] - mean;
		mean_4 += (float)pow((double)k, 4.0);
	}
	
	float kurtosis = mean_4/(n*(float)pow((double)standard_deviation, 4.0));
	return kurtosis; 
}
