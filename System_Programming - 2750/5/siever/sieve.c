//Ervin Hennrich
//cs 2750


#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	//for use later in the loops 
	unsigned int i,p;
	int *primes;
	long long int limit = 100000;	
	int sqlimit;

	//allocate array, mallocing the array with size of int is probably overkill since the elements are just place holders
	primes = malloc(sizeof(int) * limit);
	
	sqlimit = sqrt(limit);
	
	//count up from 2 to N (limit)
	//not really necessary to have the actuall number in the array element
	for (i = 2; i <= limit; i++)
		primes[i] = 1; //store a 1 in each element in the array
	//Read through the array again
	for (i = 2; i <= sqlimit; i++)  //stops at the sqrt of the limit
		if (primes[i] == 1) //check each element of the array for a 1
			for (p = i; i * p < limit; p++) //multiply to find all multiples of
				primes[i*p]=0;
	//then count through the array and print out the i for every 1 left
	printf("\nPrime numbers from 1 to 100,000 are: \n");
	for (i = 2; i < limit; i++)
		if (primes[i])
			printf("%d\n", i);

return 0;
}
