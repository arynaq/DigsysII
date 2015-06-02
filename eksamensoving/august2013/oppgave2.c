#include <stdio.h>

float minTemp[365];
float maxTemp[365];
int nedbor[365];

int main(int argc, char const *argv[])
{
	/*Deloppgave a*/
	int dag = 0;
	int max = 0;
	for(int i=0; i<365; i++){
		if(maxTemp[i]>max){
			max = maxTemp[i];
			dag = i;
		}
	}
	printf("Maxtemperatur, %d celcius, inntreffer på dag %d\n", max, dag);

	/*Deloppgave b*/
	int tropicalNights = 0;
	for(int i=0; i<365; i++){
		if(minTemp[i]>=20) tropicalNights++;
	}
	printf("Antall tropenatt: %d\n", tropicalNights);

	/*Deloppgave c*/
	int snowDays = 0;
	for(int i=0; i<365; i++){
		if(nedbor[i]>0 && maxTemp[i]<0) snowDays++;
	}
	printf("Antall dager med snø: %d\n", snowDays);
	return 0;
}