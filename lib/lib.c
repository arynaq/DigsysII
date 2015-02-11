#include <math.h>
#include </home/arynaq/HioA/DigsysII/lib/lib.h>

double bisectionMethod(double a, double  b, int maxIter,  double (*f)(double)){
  return bisectionMethodActual(a,b,0,maxIter,f);
}

static double bisectionMethodActual(double a, double b, int currIter,
				    int maxIter, double (*f)(double)){
  double c = (a+b)/2;
  double tol = 1E-7;
  
  if(fabs(f(c)) < tol || currIter == maxIter) 
    return c;
  
  if(f(c) * f(b) > 0)
    b = c;
  else
    a = c;

  currIter++;

  return bisectionMethodActual(a,b,currIter,maxIter,f);
}


