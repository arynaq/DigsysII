#include <stdio.h>

int main(int argc, char** argv)
{
	float price = 27.50;
	int personer;

	printf("Skriv inn antall guests: ");
	scanf("%d", &personer);

	char* test = "TEst";

	printf("Test er: %s", test+1);

	printf("Totalkostand for %d personer er: %.2f\n", personer, personer*price);
	return 0;
}