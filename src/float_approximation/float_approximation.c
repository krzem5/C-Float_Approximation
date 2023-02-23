#include <float_approximation.h>



typedef union _FLOAT_DATA{
	float f;
	unsigned int v;
} float_data_t;



int approximate_float(float f,unsigned int* out){
	float_data_t float_data={
		.f=f
	};
	unsigned int shift=(float_data.v>>23)&0xff;
	unsigned int numerator=(float_data.v&0x7fffff)|0x800000;
	unsigned int denominator=1;
	if (shift>181){
		*out=1;
		return (float_data.v>>31?-0xffff:0xffff);
	}
	if (shift<119){
		*out=1;
		return 0;
	}
	if (shift>=150){
		numerator<<=shift-150;
	}
	else{
		denominator<<=150-shift;
	}
	unsigned int original_numerator=numerator;
	unsigned int original_denominator=denominator;
	unsigned int lower_numerator=1;
	unsigned int lower_denominator=0;
	unsigned int upper_numerator=0;
	unsigned int upper_denominator=1;
	while (denominator){
		unsigned int scale=numerator/denominator;
		unsigned int new_lower_numerator=upper_numerator+scale*lower_numerator;
		unsigned int new_lower_denominator=upper_denominator+scale*lower_denominator;
		if (new_lower_numerator>0xffff||new_lower_denominator>0xffff){
			break;
		}
		upper_numerator=lower_numerator;
		upper_denominator=lower_denominator;
		lower_numerator=new_lower_numerator;
		lower_denominator=new_lower_denominator;
		unsigned int new_denominator=numerator-scale*denominator;
		numerator=denominator;
		denominator=new_denominator;
	}
	if (!lower_denominator){
		*out=1;
		return (float_data.v>>31?-0xffff:0xffff);
	}
	unsigned int scale=(0xffff-upper_denominator)/lower_denominator;
	upper_numerator+=scale*lower_numerator;
	upper_denominator+=scale*lower_denominator;
	int lower_error=lower_numerator*original_denominator-original_numerator*lower_denominator;
	int upper_error=upper_numerator*original_denominator-original_numerator*upper_denominator;
	if (lower_error<0){
		lower_error=-lower_error;
	}
	if (upper_error<0){
		upper_error=-upper_error;
	}
	if (lower_error<upper_error){
		*out=lower_denominator;
		return (float_data.v>>31?-lower_numerator:lower_numerator);
	}
	*out=upper_denominator;
	return (float_data.v>>31?-upper_numerator:upper_numerator);
}
