#include <stdio.h>

int max(int ,int);

int main(int argc, char** argv){
  printf("\nMax of 5 and 5 is: %d",max(5,5));
  printf("\nTest: %d", 1==1);
  return 0;
}

int max(int x, int y) {
  if (x>y) {
    printf("\nx");
    return x;
  }
  if (y>x){
    printf("\ny");
    return y;
  }
  
}
