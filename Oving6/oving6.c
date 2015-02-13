#include <stdio.h>


void swap(int* a, int* b, int* c);
void test_swap();

int main(int argc, char** argv){
	test_swap();
	return 0;
}

void test_swap(){
	int a = 3;
	int b = 2;
	int c = 1;
	swap(&a, &b, &c);
	printf("\nx<y<z: %d<%d<%d", a,b,c);
}

void swap(int* a, int* b, int* c){
	int tmp;
	if(*a>*b){
		tmp = *a;
		*a = *b;
		*b = tmp;
	}
	if(*b>*c){
		tmp = *b;
		*b = *c;
		*c = tmp;
	}

	if(*a>*b){
		tmp = *a;
		*a = *b;
		*b = tmp;
	}
}


