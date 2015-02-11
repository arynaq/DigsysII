#include <stdio.h>

void eksempel1(void);
void eksempel2(void);
void checkIfSkuddAar(int);


int main(void){
  int aar;
  scanf("%d", &aar);
  checkIfSkuddAar(aar);
  return 0;
}

void eksempel1(void){
}

void eksempel2(void){
}

 
void checkIfSkuddAar(int aar){
  if(aar%100==0)
    printf("\nNope the year %d", aar);
  else if(aar%4==0)
    printf("\nYepp the year %d", aar);
  else
    printf("\nNope the year %d", aar);
} 
