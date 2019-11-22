#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void phoneout (char mess[]){
	int i; //for loop
	int digs = 1; //increments once it finds a digit
	int length = strlen(mess); //the length of the string
	char phone[11]; //to store numbers I originally was casting each element of string  to int to put into an int array but this works better

	for ( i = 0; i < length; i++){
		char f = mess[i];
		if ( isdigit(f)) { //checks if its a digit
			if ( digs < 11) {
				phone[digs] = f; //stores digit
				digs++; //increments digs 
				}
			}
		}
	printf("\n(%c%c%c) %c%c%c-%c%c%c%c", phone[1], phone[2], phone[3], phone[4], phone[5], phone[6], phone[7], phone[8], phone[9], phone[10]);
}



