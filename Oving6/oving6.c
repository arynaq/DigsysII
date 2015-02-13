#include <stdio.h>


void swap(int* a, int* b, int* c);
void min_and_max(int arr[], int n, int* max, int* min);
int first_zero(int arr[], int n);
void sort(float arr[], int n);
float median(float arr[], int n);

void test_swap();
void test_min_and_max();
void test_first_zero();
void test_sort();
void test_median();

int main(int argc, char** argv){
	test_swap();
	test_min_and_max();
	test_first_zero();
	test_sort();
	return 0;
}


float median(float arr[], int n){
	sort(arr, n);
	if(n%2 !=0)
		return arr[n/2];

	float ret = (arr[n] + arr[n-1])/2.0;
	return ret;
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
void min_and_max(int arr[], int n, int* max, int* min){
	*max = *arr;
	*min = *arr;

	for(int i=0; i<n; i++){
		if(arr[i] >= *max)
			*max = arr[i];
		if(arr[i] <= *min)
			*min = arr[i];
	}
}
int first_zero(int arr[], int n){
	for(int i=0; i<n; i++){
		if(*(arr+i) == 0)
			return i;
	}
	return -1;
}
void sort(float arr[], int n){
	int tmp;
	for(int i=0; i<n-1; i++){
		for(int j=0; j<n-1; j++){
			if(arr[j+1]>=arr[j]){
				tmp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = tmp;
			}
		}
		n--;
	}
}



void test_median(){
	
}

void test_sort(){
	printf("\n\nTesting sorting of float array, in descending order.");
	float arr[] = {18.0, -27.3, 0.0, 29301.381, -2891.2819, -2.8};
	int n = sizeof arr / sizeof *arr;

	printf("\nFor array: ");
	for(int i=0; i<n; i++){
		printf("\n %f", arr[i]);
	}
	sort(arr,n);
	printf("\nAfter sort:");
	for(int i=0; i<n; i++){
		printf("\n %f", arr[i]);
	}
}
void test_swap(){
	printf("\n\nFunction swap() testing. Sort 3,2,1 -> 1,2,3 by changing value of pointers");
	int a = 3;
	int b = 2;
	int c = 1;
	printf("\nBefore swap: %d %d %d", a,b,c);
	swap(&a, &b, &c);
	printf("\nAfter swap: %d<%d<%d", a,b,c);
	
}
void test_min_and_max(){
	printf("\n\nTesting function min_and_max() that takes an array and"
			 "returns the max and min elements in the pointer paramters");

	int a[] = {-238,172661,2716,282673,9727,16263,0,81763,28612,-827131};
	int n = sizeof a / sizeof *a;
	int max, min;

	min_and_max(a,n,&max,&min);

	printf("\n For the array: ");
	for(int i=0; i<n; i++){
		printf("\n %d", a[i]);
	}
	printf("\n [Max,Min]: [%d, %d]", max,min);
}
void test_first_zero(){
	printf("\n\nTesting first_zero(), takes an array and returns the index of the first zero, -1 if no zeros found.");
	int arr[] = {1,2,3,4,5,0,17,0};
	int n = sizeof arr / sizeof *arr;

	printf("\nFor the array: ");
	for(int i=0; i<n; i++){
		printf("\n%d", arr[i]);
	}
	printf("\nZero is index %d.", first_zero(arr,n));

}


