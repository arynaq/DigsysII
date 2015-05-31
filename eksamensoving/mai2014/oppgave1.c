#include <stdio.h>


int barn;
int senior;
int voksne; 

int barnPris = 150;
int voksnePris = 200;
int seniorPris = 100;
int comboPris = 50;

int totalPris = 0;

int main(int argc, char const *argv[])
{
	printf("Antall barn: "); scanf("%d", &barn);
	printf("Antall voksne: "); scanf("%d", &voksne);
	printf("Antall seniorer: "); scanf("%d", &senior);

	int maxSeniorCombo = barn*4;
	senior = maxSeniorCombo <= senior ? (senior-maxSeniorCombo) : senior;

	totalPris = maxSeniorCombo*comboPris + barn*barnPris + voksne*voksnePris + senior*seniorPris;

	printf("Totalpris: %d\n", totalPris);

	return 0;
}