#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void secondDegPolyRoot(void);
void dataStats(void);
void interactiveAddition(void);
void guessNumber(void);
void dicesAndSlices(int);


int main(int argc, char** argv){

  dicesAndSlices(103500);
  return 0;
}


void dicesAndSlices(int rolls){
  srand(time(NULL));
  int sums[11] = {0};  //Initializes all elements to 0 
  int diceA; int diceB;
  
  for(int i=0; i<rolls; i++){
    diceA = rand()%6 + 1;
    diceB = rand()%6 + 1;

    //0th element is 2
    //1th element is 3
    //etc
    sums[diceA+diceB-2] += 1;
  }


  printf("\nDouble dice, rolls, [Sum, Frequency, Percent of total]");
  for(int i=0; i<11; i++){
    printf("\n[%d, %d, %.2f]", i+2, sums[i], 100.0*sums[i]/rolls);
  }
  printf("\n");
}


/**
   A random number is picked, in the range 0<N<100.
   User keeps guessing, with hints if he is wrong, until he is right.
 */

void guessNumber(void){
  printf("\nI picked a number, guess what it is.");
  srand(time(NULL));
  int guess;
  int n = rand()%100;
  
  while(guess != n){
    printf("\nGuess: "); scanf("%d", &guess);
    if(guess>n)
      printf("\nToo big");
    else if(guess<n)
      printf("\nToo small");
  }
  
  printf("Congratulations, correct answer!\n");
}

void interactiveAddition(void){
  srand(time(NULL));
  int a; int b; int c; int guess;

  a = rand()%100000;
  b = rand()%100000;
  c = a + b;

  printf("\n%d + %d =?", a, b);
  printf("\nGuess: "); scanf("%d", &guess);

  if(guess == c)
    printf("\n You guessed right!");
  else
    printf("\n You guessed wrong!, correct answer is: %d\n", c);

}

/**
Finds the roots of a second degree polynomial
*/

void secondDegPolyRoot(void){
  double a; double b; double c;
  double x1; double x2;

  printf("\n Finds the roots of a polynomial in the form ax**2 + bx + c");

  printf("\n Enter a: "); scanf("%lf", &a);
  printf("\n Enter b: "); scanf("%lf", &b);
  printf("\n Enter c: "); scanf("%lf", &c);
  
  if(b*b - 4*a*c < 0) {
    x1 = -b/(2*a);
    x2 = -b/(2*a);

    double im = sqrt(fabs(b*b - 4*a*c))/(2*a);

    printf("\nx1= %lf + i%lf", x1, im);
    printf("\nx2= %lf - i%lf\n", x2, im);
  }

  else{
    x1 = (-b + sqrt(b*b - 4*a*c))/(2*a);
    x2 = (-b - sqrt(b*b - 4*a*c))/(2*a);

    printf("\nx1= %lf", x1);
    printf("\nx2= %lf\n", x2);
  }

}

/**
   Reads in numbers from the commandline and prints out the average, min, max; 
 */
void dataStats(void){
  double buffer[1024];
  char   cBuffer[1024];
  int    counter;
  double min; double max; double sum; double current; long maxSet; long minSet;

  // Keep reading as long as input is different from "e"

  while(1){
    printf("\nNumber: ");scanf("%s", cBuffer);
    
    if(strcmp(cBuffer, "e") == 0)
      break;
    
    current = atof(cBuffer);
    buffer[counter++] = current;
    sum += current;

    //If current input is lower than the min or min has never been set, set current to min
    if(current < min || minSet++==0)
      min = current;
    //If current input is higher than the max or max has never been set, set current to max
    if(current > max || maxSet++==0)
      max = current;

    //Make sure there is enough space in the buffer
    if(counter == sizeof(buffer)/sizeof(double)) {
      double newBuffer[2* sizeof(buffer)/sizeof(double)];
      memcpy(newBuffer, buffer, sizeof(buffer));
    }
  }

  printf("\nMin: %lf", min);
  printf("\nMax: %lf", max);
  printf("\nAverage: %lf\n", sum/counter); 
  
}


