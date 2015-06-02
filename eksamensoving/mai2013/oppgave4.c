#include <stdio.h>

float volum(float, float);

float radius[5] = {1.0, 2.17, 8.71, 1.17, 0.31};
float hoyde[5] = {0.1, 86.2, 1.871, 0.01, 5.0};


int main(int argc, char const *argv[])
{
	/*Deloppgave b */

	float sylinder[5];

	for(int i=0; i<5; i++){
		sylinder[i] = volum(radius[i], hoyde[i]);
	}

	int idx[5] = {0,1,2,3,4};

	for(int i=0; i<5; i++){
		for(int j=0; j<5; j++){
			if(sylinder[j]>sylinder[i]){
				float tmp = sylinder[i];
				int tmpIdx = idx[i];
				idx[i] = idx[j];
				idx[j] = tmpIdx;
				sylinder[i] = sylinder[j];
				sylinder[j] = tmp;
			}
		}
	}


	for(int i=0; i<5; i++){
		printf("Radius, Hoyde, Volum: [%.2f, %.2f, %.2f]\n",radius[idx[i]], hoyde[idx[i]],sylinder[i]); 
	}

	return 0;
}


float volum(float radius, float hoyde){
	return 3.14*radius*radius*hoyde;
}