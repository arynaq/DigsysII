#include <math.h>
#include </home/arynaq/HioA/DigsysII/lib/lib.h>
#include <stdexcept>

double bisectionMethod(double a, double  b, int maxIter,  double (*f)(double)){
  return bisectionMethodActual(a,b,0,maxIter,f);
}


double newtonsMethod(double a, double b, double x0, int maxIter, double (*f)(double), double (*df)(double)){

  return newtonsMethodActual(a,b,x0,0,maxIter,f,df);  
}

static double newtonsMethodActual(double a, double b, double x0, int currIter, int maxIter,
                            double (*f)(double), double (*df)(double)){
  

  double x1 = x0 - f(x0)/df(x0);  
  double fVal = f(x1);

  if(fabs(fVal)<1E-5)
    return x1;
  if(currIter == maxIter)
    throw std::overflow_error("Too many iterations!");

  currIter++;

  return newtonsMethodActual(a,b,x1,currIter,maxIter,f,df);
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


