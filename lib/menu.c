#include <stdio.h>
#include <string.h>

char* testA(void);
char* testB(void);
void testC(char**);

void listMenu(int argc, char* (**p) (void));

int main(int argc, char** argv){
  
  char* (*p[2]) (void);

  p[0] = testA;
  p[1] = testB;

  listMenu(2,p);


  char* c = "This is the old string!";
  testC(&c);
  printf("\n%s", c);

}



void testC(char** a){
  *a = "Replaced!";
}


void listMenu(int argc, char* (**p)(void)){
  char buffer[256];

  for(int i=0; i<argc; i++)
    printf("\nPress %d for %s", i, (*p[i])());
  printf("\nPress m for this menu.");
}

char* testA(void){
  char* ret = "This is a test: A";
  return ret;
}

char* testB(void){
  char* ret = "This is a test: B";
  return ret;
}





