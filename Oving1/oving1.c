#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>


char* oppgave1(void);
char* oppgave2(void);
char* oppgave3(void);
char* oppgave4(void);
char* oppgave5(void);
void displayMenu(void);
void readInAndChoose(void);

char readIn[256];

int  main(int argc, char** argv){
  displayMenu();
  srand(time(NULL));
  while(1)
    {
      readInAndChoose();
    }
  return 0;
}




char* oppgave1(void){
  char* ret = "Celcius to fahrentheit calculator";
  double c; double f;

  printf("\nEnter degrees celcius: "); scanf("%lf", &c);
  f = (9.0/5)*c + 32;

  printf("\n%lf degrees celcius is %lf fahrenheit", c,f);
  return ret;
}

char* oppgave2(void){
  char*ret = "Calculate distance between two planar points [x0,x1] and [y0,y1]";
  int x[2]; int y[2];

  printf("%s", ret);  
  printf("\nx1: "); scanf("%d", &x[0]);
  printf("\nx2: "); scanf("%d", &x[1]);
  
  printf("\ny1: "); scanf("%d", &y[0]);
  printf("\ny2: "); scanf("%d", &y[1]);

  double dx = y[0] - x[0];
  double dy = y[1] - x[1];
  double r = sqrt(dx*dx + dy*dy);

  printf("Distance between points [%d,%d] and [%d,%d] is %f",
	 x[0],x[1], y[0], y[1], r);

  return ret;
}

char* oppgave3(void){
  int s; int m;
  char* ret = "Convert seconds to whole minute(s)";
  
  printf("%s",ret);
  printf("\nEnter seconds: "); scanf("%d", &s);
  m = s/60;
  s = s%60;
  printf("%dm%ds", m, s);
}

char* oppgave4(void){
  char* ret = "Multiplication quiz!";
  int a = rand()%11; 
  int b = rand()%11; 
  int x;

  printf("%s",ret);
  printf("\n What is the product of %d and %d?", a, b);
  printf("\nAnswer: "); scanf("%d", &x);

  if(x==(a*b))
    printf("\nCorrect!");
  else
    printf("\nWrong answer, the product is %d", (a*b));

  return ret;
}

char* oppgave5(void){
  char* ret = "Crossum calculator for positive integers, eg. 53 -> 8";
  int sum = 0;
  int number =0;
  
  printf("\n Enter number: "); scanf("%d", &number);

  int tmp = number;
  while(number>0){
    sum += number % 10;
    number = number/10;
  }
  printf("\n The crossum of the integer %d is %d", tmp, sum);

  return ret;
}

void readInAndChoose(void){
  scanf("%s", readIn);

  if(strcmp(readIn, "1") == 0)
    oppgave1();
  else if (strcmp(readIn, "2") == 0)
    oppgave2();
  else if (strcmp(readIn, "3") == 0)
    oppgave3();
  else if (strcmp(readIn, "4") == 0)
    oppgave4();
  else if (strcmp(readIn, "5") == 0)
    oppgave5();
  else if (strcmp(readIn, "m") == 0)
    displayMenu();
}


void displayMenu(void){
  printf("Enter 1 for numbercalculator.");
  printf("\nEnter 2 for distancecalculator.");
  printf("\nEnter 3 for seconds to minutes conversion.");
  printf("\nEnter 4 for interactive multiplicationtable.");
  printf("\nEnter 5 for the crossum-calculator.");
  printf("\nEnter m for this menu..");
  printf("\n");
}




