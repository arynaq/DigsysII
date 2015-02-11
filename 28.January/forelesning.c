#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void test(void);
double monteCarloIntegration(double, double, int, double (*f)(double));
double f(double);

int main(int argc, char** argv){
  test();
  return 0;
}



void test(void){
  printf("\n");
  printf("Integral: %lf", monteCarloIntegration(0,1,100,f));
}

double monteCarloIntegration(double a, double b, int points,
			     double (*f)(double)){
  
  double sum = 0.0; double sample;
  int width = (b-a)/points;


  for(int i=0; i<points; i++){
    sample = 1.0 * rand() / RAND_MAX * width + a;
    sum+= f(sample);
  }


  return sumc/points;
}


double f(double x){
  return sin(x);
}


