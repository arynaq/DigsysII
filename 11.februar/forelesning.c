#include <stdio.h>


void test(int*, int*);
void swap5(int**, int**);

int main(int argc, char** argv){
	int a = 10;
	int b = 20;

	int *pa = &a;
	int *pb = &b;


	test(&a,&b);

	return 0;
}

void test(int *x, int *y){

	int a[][2] = {{12,3},{22,3},{33,3}};

	printf("\nvalue of (a+1)[0] %d\n", (a+1)[0]);
	printf("\nvalue of a(+2)[0] %d\n", (a+2)[0]);

}

void swap5(int** a, int** b) {
    int* temp = *a;
    *a = *b;
    *b = temp;
}





