#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char const *argv[])
{	

	char s[] = "This is a string";
	char* p = strtok(s, " ");

	while(p != NULL){
		printf("%s\n", p);
		p = strtok(NULL, " ");
	}
	return 0;
}