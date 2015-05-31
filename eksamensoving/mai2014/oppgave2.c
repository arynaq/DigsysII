#include <stdio.h>

int nedbor[365];


int main(int argc, char** argv){

	/*deloppgave a */
	int dagerUtenNedbor = 0;
	for(int i=0; i<sizeof nedbor/ sizeof *nedbor; i++){
		if(nedbor[i] == 0) dagerUtenNedbor++;
	}
	printf("Dager uten nedbør: %d\n", dagerUtenNedbor);

	/*deloppgave b*/
	int maxNedbor = 0;
	int dagMaxNedbor = 0;

	for(int i=0; i<sizeof nedbor / sizeof *nedbor; i++){
		if(nedbor[i]>maxNedbor){
			dagMaxNedbor = i;
			maxNedbor = nedbor[i];
		}
	}

	printf("Det var mest nedbør på dag: %d\n", dagMaxNedbor);
	printf("Da var nedbøren: %d\n", maxNedbor);	

	/*deloppgave c*/
	int nextDayNedborCount = 0;
	int nextDayNoNedborCount = 0;

	for(int i=0; i<sizeof nedbor / sizeof *nedbor -1; i++){
		if(nedbor[i]>0) {
			if(nedbor[i+1]>0) nextDayNedborCount++;
			else nextDayNoNedborCount++;
		}
	}

	printf("Antall nedbørsdager etterfulgt av nedbør: %d\n", nextDayNedborCount);
	printf("Antall nedbørsdager etterfult 0 nedbør: %d\n", nextDayNoNedborCount);

	return 0;
}
