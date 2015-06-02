###fgets(char* str, int n, FILE* stream);
The C library function char *fgets(char *str, int n, FILE *stream) reads a line from the specified stream and stores it into the string pointed to by str. It stops when either (n-1) characters are read, the newline character is read, or the end-of-file is reached, whichever comes first.
###gets(char* str, int n);
The C library function char *gets(char *str) reads a line from stdin and stores it into the string pointed to by str. It stops when either the newline character is read or when the end-of-file is reached, whichever comes first.
###scanf(const char* format, ....)
The C library function int scanf(const char *format, ...) reads formatted input from stdin.

	#include <stdio.h>

	int main(){
   	 char str1[20], str2[30];

     printf("Enter name: ");
     scanf("%s", &str1);

     printf("Enter your website name: ");
     scanf("%s", &str2);

     printf("Entered Name: %s\n", str1);
     printf("Entered Website:%s", str2);
   
     return(0);
	}

###fscanf(FILE *stream, const char *format, ...)
The C library function int fscanf(FILE *stream, const char *format, ...) reads formatted input from a stream.
	#include <stdio.h>
	#include <stdlib.h>


	int main()
	{
	   	char str1[10], str2[10], str3[10];
	   	int year;
	   	FILE * fp;

	   	fp = fopen ("file.txt", "w+");
	   	fputs("We are in 2012", fp);
	   
	   	rewind(fp);
	   	fscanf(fp, "%s %s %s %d", str1, str2, str3, &year);
	   
	   	printf("Read String1 |%s|\n", str1 );
	   	printf("Read String2 |%s|\n", str2 );
	   	printf("Read String3 |%s|\n", str3 );
	   	printf("Read Integer |%d|\n", year );

	   	fclose(fp);
	   
	   	return(0);	
	}
###fprintf(FILE* stream, const char *format, ....)
The C library function int fprintf(FILE *stream, const char *format, ...) sends formatted output to a stream.
	#include <stdio.h>
	#include <stdlib.h>

	int main()
	{
	   FILE * fp;

	   fp = fopen ("file.txt", "w+");
	   fprintf(fp, "%s %s %s %d", "We", "are", "in", 2012);
	   
	   fclose(fp);
	   
	   return(0);
	}