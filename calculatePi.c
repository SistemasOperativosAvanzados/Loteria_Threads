
#include <math.h>
#include <stdio.h>


void calculatePi(int  units){
	float pi = 0;
	for (int k = 0 ; k < (50*units);k++){
		pi = pi + (pow(-1,k))/(2*k+1);
		
	}

	printf("%.10f\n",4* pi);
}

int main(int argc, char const *argv[])
{
	/* code */
	calculatePi(5000);
	return 0;
}