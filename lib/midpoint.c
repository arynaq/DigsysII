#include <stdio.h>
#include <math.h>


double midPointZero(double, double, double (*f)(double));
double testFunc(double);
double testFunc2(double);
int iter;

int main(int argc, char** argv){
  printf("\nZero: %lf", midPointZero(-2,3.14, testFunc2));
  printf("\nIterations: %d\n", iter);
  return 0;
}



double midPointZero(double a, double  b, double (*f)(double)){
  double c = (a+b)/2;
  double tol = 1E-5;
  iter++;
  
  if(fabs(f(c)) < tol) 
    return c;
  
  if(f(c) * f(b) > 0)
    b = c;
  else
    a = c;

  return midPointZero(a,b,f);
    
}

double testFunc(double x){
  return x*x - 1;
}

double testFunc2(double x){
  return sin(x) - 0.5;
}

