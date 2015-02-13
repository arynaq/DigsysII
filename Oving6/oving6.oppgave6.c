#include <stdio.h>
#include <time.h>
#include <stdlib.h>


int main(int argc, char** argv){
	srand(time(NULL));

	int rain[12][31] = {{0}};
	int daysWithRaind

	for(int i=0; i<12; i++){
		for(int j=0; j<31; j++){
			int _rain = rand()%10 +1;
			if(_rain>6)
				continue;
			rain[i][j] = _raind;
		}
	}

	return 0;
}