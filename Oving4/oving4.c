#include <math.h>
#include <stdio.h>

/*Calculates powers, i, i^2, i^3 */
void powers(void);
/*Calculates the factorial of a number */
int factorial(int);
/*Calculates the area of a 3 sided rectangle */
float areal(float, float, float);
/*Calculates the greatest commmon denominator */
int gcd(int,int);


int main(int argc, char** argv){
  printf("\n%d \n",gcd(655116, 136875));
  return 0;
}


int gcd(int a, int b){
  if(a==b)
    return a;
  if(a>b)
    return gcd(a-b,b);
  else
    return gcd(a,b-a);
}


float areal(float s1, float s2, float s3){
  float s = (s1+s2+s3)/2;
  float a = sqrt(s*(s-s1)*(s-s2)*(s-s3));
  return a;
}


int factorial(int a){
  if(a==1)
    return 1;
  return a*factorial(a-1);
}

void powers(void){
  printf("\n%10s %10s %10s", "i", "i*i", "i*i*i\n");
  for(int i=1; i<=10; i++){
    printf("\n %10d %10d %10d", i, i*i, i*i*i);
  }
}
