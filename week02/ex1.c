#include <stdio.h>
#include <limits.h>
#include <float.h>

int main(void) {
	int integer;
	unsigned short int u_s_integer;
	signed long int s_l_integer;
	float float_num;
	double double_num;
	
	integer = INT_MAX;
	u_s_integer = USHRT_MAX;
	s_l_integer = LONG_MAX;
	float_num = FLT_MAX;
	double_num = DBL_MAX;
	
	printf("size of int: %lu bytes\nvalue of MAX_INT: %d\n", sizeof(integer), integer);
	printf("size of unsigned short int: %lu bytes\nvalue of USHRT_MAX: %hu\n", sizeof(u_s_integer), u_s_integer);
	printf("size of signed long int: %lu bytes\nvalue of LONG_MAX: %ld\n", sizeof(s_l_integer), s_l_integer);
	printf("size of float: %lu bytes\nvalue of FLT_MAX: %f\n", sizeof(float_num), float_num);
	printf("size of double: %lu bytes\nvalue of DBL_MAX: %lf\n", sizeof(double_num), double_num);
	    
    	return 0;
}
