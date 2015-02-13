#include <stdio.h>
#include <string.h>

void toUniques(int* arr ,int* ret, int n, int m);
void test_toUniques(void);

int main(int argc, char** argv){
	test_toUniques();
	return 0;
}

void test_toUniques(){
	int a[] = {1,3,3,3,3,3,12,58917,1762,271,817,217,3,3,58};
	int n = sizeof a / sizeof *a;
	int b[n];
	memset(b,0,sizeof b);


	toUniques(a,b,n,n);
	for (int i = 0; i<n; i++)
	{
		printf("\n%d\t %d",a[i], b[i]);
	}
}

void toUniques(int* arr, int* ret, int n, int m){
	int nUnique = 0;
	int found = 0;

	for(int i=0; i<n; i++){
		found = 1;
		for(int j=0; j<nUnique; j++){
			if(arr[i] == ret[j]){
				found = 0;
			}
		}

		if(found){
			ret[nUnique++] = arr[i];
		}
	}
}