#include <stdio.h>



struct linje {
	float a;
	float b;
}L1 = {2,3}, L2 = {3,4};

float skjaringspunkt(float, float, float, float);
float skjaringspunkt2(struct linje, struct linje);



int main(int argc, char const *argv[])
{
	float a1, a2, b1, b2, x;

	printf("Skriv inn a1,b1: ");
	scanf("%f %f", &a1, &b1);
	printf("Skriv inn a2, b2: ");
	scanf("%f %f", &a2, &b2);

	
	printf("Skjæringspunktet, x, er: %.3f\n", skjaringspunkt(a1,b1,a2,b2));	

	printf("Skjæringspunktet, x, er: %.3f\n", skjaringspunkt2(L1,L2));
	return 0;
}

float skjaringspunkt(float a1, float b1, float a2, float b2){
	if(a1 == a2) return 99999;

	return (b2-b1)/(a1-a2);
}

float skjaringspunkt2(struct linje L1, struct linje L2){
	if(L2.a == L1.a) return 99999;

	return (L2.b-L1.b)/(L1.a - L2.a);
}