#include "xaccel_ygyro.h"

void get_ygyro(const double* data_val, const int data_buf_size, double *y_gyro) //data_val is complete //data num is size
{
	int j;
	for (j = 0; j < data_buf_size; j++)                     //might need to be data_buf_size -1
   y_gyro[j] = data_val[j*_DATA_ACQ_SIZE + _GYRO_Y_OFFSET];
}


//individual will need to stand still to eliminate offset of gravity
//double or float, whatever it is
double get_gravity_offset( double x_accel_data[], int iterations) {
int total = 0;
int x = 0;
while( x < iterations)
{
	total += x_accel_data[x];
	x++;
}
return total/x; 
}



// takes an xaccel segment and eliminates offset
void eliminate_offset( double *x_accel_data, int segment_length, double gravity_offset ) {
	int i, j; 
	for (i =0; i < segment_length; i++){
		x_accel_data[i] += gravity_offset;  //should be -= if using real time and not macro 
	}
}

void execute_lowpass_filter( double x_accel_data[], int segment_length, double* filtered_data  ){
	float f0 = 0f; //to be ignored
	float As = 0f; //to be ignored
	float Ap = 0f; //to be ignored 
	iirfilt_rrrf q = iirfilt_crcf_create_prototype( LIQUID_IIRDES_BUTTER,
													LIQUID_IIRDES_LOWPASS, 
													LIQUID_IIRDES_SOS,
													LP_FILTER_ORDER,
													CUTOFF_FREQ_L, f0, Ap, As); 											
	iirfilt_crcf_execute(q, x_accel_data, filtered_data); 
	iirfilt_crcf_destroy(q); 													
}


void execute_highpass_filter( double input[], int segment_length, double* filtered_data) {
	float f0 = 0f; //to be ignored
	float As = 0f; //to be ignored
	float Ap = 0f; //to be ignored 
	iirfilt_rrrf q = iirfilt_crcf_create_prototype( LIQUID_IIRDES_BUTTER,
													LIQUID_IIRDES_HIGHPASS, 
													LIQUID_IIRDES_SOS,
													LP_FILTER_ORDER,
													CUTOFF_FREQ_H, f0, Ap, As); 											
	iirfilt_crcf_execute(q, input, filtered_data); 
	iirfilt_crcf_destroy(q); 													
}
double y_gyro_features_1( const double* segment, int begin, int end){
	return w_minima_double_seg(segment, begin, end);
}

void y_gyro_features_2( const double* segment, int segment_length, int begin, int end, double* abs_max, double* rel_min, double* rel_max)
{
	//find absolute max
    int c, index;
    double max;
    max = segment[begin];
    index = begin;
    for (c = index; c < end; c++) {
        if (segment[c] > max) {
            index = c;
            max = segment[c];
        }
    }
	*abs_max = segment[index]; 
	
	//start at least 20 points away for next maximum, could be 25 or 50 
	//or find a better way to do this 
	int index_2 = index +=20; 
	max = 0;
	for( c = index_2; c < end; c++) {
		if(segment[c] > max) {
			index_2 = c; 
			max = segment[c]; 
		}
	}
	*rel_max = segment[index_2];
	
	//find minimum between them 
	*rel_min = w_minima_double_seg(segment, index, index_2);
}

void create_ygyro_feature_array(double* ygyro_data, double* ygyro_features, int* seg_val, size_t seg_num)
{
	double y_gyro_abs_min;
	int i;
	for (i = 0; i < seg_num; i++)
	{
		y_gyro_abs_min = y_gyro_features_1(ygyro_data, seg_val[i], seg_val[i + 1]);
	}
}