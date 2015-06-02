#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


int deltagere;
int andebrystVekt = 300;
int kiloAndeBryst;

int main(int argc, char const *argv[])
{	
	printf("Skriv inn antall deltagere: ");
	scanf("%d", &deltagere);

	kiloAndeBryst = ceil(deltagere*andebrystVekt/1000.0);

	printf("Trenger totalt %dkg andebryst\n", kiloAndeBryst); 

	srand(time(NULL));

	/*Deloppgave b */
	srand(time(NULL));
	for(int i=0; i<10000; i++) printf("%d\n", rand()%14 + 1);
	return 0;
}