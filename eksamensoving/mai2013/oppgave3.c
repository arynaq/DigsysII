#include <stdio.h>
#include <string.h>


int main(int argc, char const *argv[])
{
	FILE* f;
	FILE* fr;

	if((f=fopen("medlemmer.txt", "r"))==NULL || (fr = fopen("redigert.txt", "w"))==NULL){
		printf("Kunne ikke åpne file(ne)\n");
		return -1;
	}

	char line[100];
	char fornavn[30];
	char etternavn[30];

	while(!feof(f)){
		fgets(line,99,f);

		if(strchr(line, ',') != NULL){
			char* p = strtok(line, ", ");
			sprintf(etternavn,"%s", p);
			p = strtok(NULL, ", ");

			fprintf(fr, "%s %s\n", p, etternavn);
		}

		else
			fputs(line, fr);
	}


	fclose(f); fclose(fr);
	return 0;
}