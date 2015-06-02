#include <stdio.h>
#include <math.h>

int members;
int pancakes;

int flour = 50;
int eggs = 1;
int milk = 2;

int main(int argc, char const *argv[])
{
	printf("Skriv inn antall medlemmer: ");
	scanf("%d",&members);

	/*Deloppgave a */
	printf("Trenger mel: %d gram\n",flour*members);
	printf("Trenger melk: %d dl\n", milk*members);
	printf("Trenger egg: %d stykker\n", eggs*members);

	/*Deloppgave b */
	float milkCost = ceil(members*milk/10.0) * 14.40;
	float flourCost = ceil(members*flour/1000.0) * 14.30;
	float eggCost = ceil(members*eggs/6.0) * 28.70;

	printf("Totalkostand: %.2f\n",milkCost+flourCost+eggCost);
	return 0;
}