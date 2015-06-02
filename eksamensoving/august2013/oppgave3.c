#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* filnavn = "tempdata.txt";
const char* modus = "r";

const int min = -50;
const int max = 40;

FILE* f;


int main(int argc, char const *argv[])
{
	if((f=fopen(filnavn,modus))==NULL){
		printf("File could not be opened.\n");
		return -1;
	}

	char datoBuffer[256];
	char minBuffer[256];
	int  minRead = 0;
	char maxBuffer[256];
	int  maxRead = 0;

	fpos_t position;

	while(strstr(datoBuffer, "datafelt") == NULL)
		fscanf(f, "%s", datoBuffer);

	fgetpos(f, &position);

	/*Deloppgave a*/
	while(!feof(f)){
		fscanf(f,"%s %d %d", datoBuffer, &maxRead, &minRead);
		if(minRead < min || maxRead > max){
			printf("Invalid data found, valid range is (%d,%d): \n", min,max);
			printf("%s %d %d\n", datoBuffer, minRead, maxRead);
		}
	}

	/*Deloppgave b*/
	const char* nyttFilnavn = "correctedTmp.txt";
	const char* modus = "w";
	FILE* newF;

	if((newF = fopen(nyttFilnavn, modus))==NULL){
		printf("Can not create new file.\n");
		return -1;
	}

	rewind(f);
	fsetpos(f, &position);
	
	while(!feof(f)){
		fscanf(f,"%s %d %d", datoBuffer, &maxRead, &minRead);	
		
		char* pch = strtok(datoBuffer, ".");
		fprintf(newF, "%s/", pch);
		pch = strtok(NULL, ".");
		fprintf(newF, "%s/",pch);
		pch = strtok(NULL, ".");
		fprintf(newF, "%s ", pch);

		fprintf(newF, "%d %d\n", maxRead, minRead);
	}

	fclose(f);
	fclose(newF);
	return 0;
}