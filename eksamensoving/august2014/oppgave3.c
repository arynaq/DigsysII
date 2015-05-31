#include <stdio.h>

float parabel(float, float, float, float);
void plotParabel(float, float, float, float, float, float);

int main(int argc, char const *argv[])
{
	plotParabel(1, -3, 2, -1, 4, 0.5);
	return 0;
}

float parabel(float a, float b, float c, float x){
	return a*x*x + b*x + c;
}


void plotParabel(float a, float b, float c, float min, float max, float dx){
	while(min <= max){
		printf("%f\n", parabel(a, b, c, min));
		min += dx;
	}
}