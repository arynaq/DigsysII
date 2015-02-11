#include <stdio.h>

void test(void);
void test2(void);
void meteorlogicalData(float[], float[], float[]);
void testArray(int[]);
void testReplaceNumber(int);



int main(int argc, char** argv){
  test();
  test2();
  return 0;
}


void test(void){
  int x[] = {1,2,3};
  int *y = &(*(x+1));

  *y = 15;
  
  printf("%d", x[1]);

}


void test2(void){
  printf("\nT2\n");
  int x[] = {1,2,3};
  int y = *(x+1);
  printf("\n%d %d", x,y);


  printf("\nT3\n");
  int a = 13;
  int *b  = &a;

  printf("\n%d", b);
}


void meteorlogicalData(float minTmp[], float maxTmp[], float rainfall[]){
  int N = sizeof(dayTmp)/sizeof(float);
  int daysWithoutRain = 0;
  int minTmpDay = 0;
  float minminTmp = 1000;
  int hotDaysWithoutRain = 0;
  

  for(int i=0; i<N; i++){
    if(rainfall[i]==0)
      daysWithRain++;
    if(minTmp[i] < minminTmp){
      minminTmp = minTmp[i];
      minTmpDay = i;
    }

    if((maxTmp[i]>20 || minTmp[i]> 20) && rainfall[i]==0)
      hotDaysWithoutRain++;
  }
}


void testArray(const int a[]){
  a[0] = 1000;
}

void testReplaceNumber(int a){
  *a = 13;
}
