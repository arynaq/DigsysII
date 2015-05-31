#include <stdio.h>

int main(int argc, char** argv){
	/*deloppgave a*/
	const char* filnavn = "klasse10a.txt";
	const char* modus = "r";
	FILE* f;

	if((f=fopen(filnavn, modus))==NULL){
		return -1;
	}

	return 0;
}

