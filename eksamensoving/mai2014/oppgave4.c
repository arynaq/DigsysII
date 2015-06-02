#include <stdio.h>
#include <string.h>

int main(int argc, char** argv){
	/*deloppgave a*/
	const char* filnavn = "klasse10a.txt";
	const char* modus = "r";
	FILE* f;

	if((f=fopen(filnavn, modus))==NULL){
		printf("Lesing av fil %s feilet.\n",filnavn);
		return -1;
	}


	/*deloppgave b*/
	char fornavn[30];
	char etternavn[30];
	char dato[30];

	fscanf(f,"%s",fornavn);

	if(strstr(fornavn,"/*") != NULL){
		while(strstr(fornavn,"*/") == NULL)
			fscanf(f,"%s",fornavn);
	}
	else
		rewind(f);

	while(!feof(f)){
		fscanf(f,"%s",fornavn);
		fscanf(f,"%s", etternavn);
		fscanf(f,"%s", dato);

		printf("%s %c.\n", fornavn, *etternavn);
	}

}

