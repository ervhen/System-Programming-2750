//Ervin Hennrich
//cs2750


#include <stdlib.h>
#include <stdio.h>

int main () {
	char mess[200]; //i just initialized it to 200, i figure thats enough for this
	printf("\nGive me some garbage text with numbers in it and i'll spit out a phone number\n");
	scanf("%s", mess);
	phoneout(mess);
}
