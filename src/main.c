#include <float_approximation.h>
#include <stdio.h>



int main(int argc,const char** argv){
	unsigned int denominator;
	int nominator=approximate_float(-1.12121212f,&denominator);
	printf("-1.12121212 ~= %d/%u (~= %.6f)\n",nominator,denominator,((float)nominator)/denominator);
	nominator=approximate_float(178/2233.0f,&denominator);
	printf("178/2233 ~= %d/%u (~= %.6f)\n",nominator,denominator,((float)nominator)/denominator);
	return 0;
}
