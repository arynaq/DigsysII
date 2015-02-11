#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>


int dice(void);
double CtoF(double);
void problemThree(int);
void problemOne(int);
void problemTwo(void);
void problemFour(int);

int main(int argc, char** argv) {
  srand(time(NULL));
  problemOne(3000);
  problemFour(0);
  return 0;
}

void problemFour(int n){
  double PI = 3.14;
  double step = PI/30;
  int D = 40;


  for(double angle=0; angle<PI; angle+=step) {
    problemThree((int)D*sin(angle));
  }
}


void problemThree(int n){
  for(int i=0; i<n; i++){
    printf("*");
  }
  printf("\n");
} 

void problemTwo(void){
  printf("\nCelcius %20s\n","Fahrenheit");
  for(double C=31.0; C<=38.0; C+=1){
    printf("\n%.1lf %20s %.1lf", C,"",CtoF(C));
  }
}

void problemOne(int n){
  int cntr = 0;
  for(int i=0; i<3000; i++) {
    if(dice() == 6)
      cntr++;
  }
  printf("\nOut of %d dices, %d turned up 6.", n, cntr);
}


double CtoF(double C){
  return 9.0/5 * C + 32;
}
int dice(void) {
  return rand()%6 + 1;
}
