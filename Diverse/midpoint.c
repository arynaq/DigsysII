#include <stdio.h>
#include <math.h>
#include </home/arynaq/HioA/DigsysII/lib/lib.h>


double testFunc(double);
double testFunc2(double);


int main(int argc, char** argv){
  printf("\n%lf", midPointZero(-100,100, 20, testFunc));
  return 0;
}

double testFunc(double x){
  return x*x - 1;
}

double testFunc2(double x){
  return sin(x) - x;
}

