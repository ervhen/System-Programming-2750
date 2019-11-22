#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int line() {
	int i;
	int max = 9; //max
	int min = 1;  //min
	int servtime = 0;
	int arrivetime = 0;
	srand(time(NULL));
	
//	open the output file customers
	FILE * fout;
	fout = fopen("customers", "w");
	
	//incase of failure
	if (fout == NULL){  //fout
		printf("Failure.\n");
		return EXIT_FAILURE;
	}
	
//made it run for 1hr 25 minutes, or 4500 seconds	
	for (i=1; arrivetime < 4500; i++){ //waiting to get to counter, and total waiting
		arrivetime=(rand() % (max-min + 1)) + min;
		max=max+9; //raise ceiling by 9
		min=min+9; //raise floor by 9
		servtime=(rand() % (400-100 + 1)) +100;
		fprintf( fout, "%d\t%d\n", arrivetime, servtime);
	}
	return 0;

}



