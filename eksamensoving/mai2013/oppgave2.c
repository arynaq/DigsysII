#include <stdio.h>

char * members[200];
int attendance[200];


int main(int argc, char const *argv[])
{
	int _25count = 0;
	/*Deloppgave a */
	for(int i=0; i<200; i++)
		if(attendance[i]==25) _25count++;

	printf("%d medlemmer har deltatt nøyaktig 25 ganger.\n",_25count);

	/*Deloppgave b */

	char * mostAttendant;
	int mostAttendance = 0;

	for(int i=0; i<200; i++){
		if(attendance[i]>=mostAttendance){
			printf("%s har deltatt mest, med %d deltagelse\n",mostAttendant,mostAttendance);
		}
	}

	return 0;
}