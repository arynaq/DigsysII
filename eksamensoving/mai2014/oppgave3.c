#include <math.h>
#include <stdio.h>


float areal(float, float, float);

int main(int argc, char const *argv[])
{
	float a = areal(3,4,5);

	printf("Arealet er: %f\n", a);
	return 0;
}

float areal(float s1, float s2, float s3){
	float s = (s1 + s2 + s3 )/2;
	return sqrt(s*(s-s1)*(s-s2)*(s-s3));
}