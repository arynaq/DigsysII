#include <stdio.h>
#include <stdlib.h>
#include <string.h>




char* filnavn = "studenter.txt";
char* mode = "r";
char matcher[100];
char matcher2[100];
char buffer[100];
char cpy [100];

int found = 0;
FILE* f;


struct student {
	char fornavn[30];
	char etternavn[30];
	char studnr[8];
	int opptaksaar;
};


struct student studenter[5000];

int main(int argc, char const *argv[]){

	if((f=fopen(filnavn, mode))==NULL){
		printf("Could not open the file.\n");
		return -1;
	}

	/* deloppgave a */
	/*
	if((f=fopen(filnavn, mode))==NULL){
		printf("Could not open the file\n");
		return -1;
	}

	printf("Skriv inn adgangsår: ");
	scanf("%s", matcher);
	sprintf(matcher2, " %s", matcher);


	while(!feof(f)){
		fgets(buffer, 100, f);
		if(strstr(buffer, matcher) != NULL)
			printf("%s\n",buffer);
	}
	*/

	/*deloppgave b*/
	/*
	

	int i = 0;

	while(!feof(f)){
		fgets(buffer, 100, f);
		fscanf(f, "%s", studenter[i].fornavn);
		fscanf(f, "%s", studenter[i].etternavn);
		fscanf(f, "%s", studenter[i].studnr);
		fscanf(f, "%d", &studenter[i].opptaksaar);
		i++;
	}
	*/

	printf("Skriv inn søkekriterie: ");
	scanf("%s", matcher);

	while(!feof(f)){
		fgets(buffer, 100, f);
		if(strstr(buffer,matcher)!=NULL)
			printf("%s\n",buffer);
	}


	fclose(f);
	return 0;
}