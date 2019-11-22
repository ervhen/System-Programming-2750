//Ervin Hennrich
//cs2750

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef enum { clubs, diamonds, hearts, spades} suit_t;

typedef struct {
	unsigned int number : 4;
	suit_t 	       suit : 2;
} card_t;

//reads the suit and returns corrosponding letter
char * getSuit(int suit_t) {
	switch(suit_t) {
		case diamonds: return "D";
		case hearts: return "H";
		case clubs: return "C";
		case spades: return "S";
	}
	return NULL;
}

//self explanitory, outputs the associated string if it is 11-13 or 1

void  getNumber(int x) {
	if ( x == 11 )
		printf("%s", "Jack");
	if ( x == 12 )
		printf("%s", "Queen");
	if ( x == 13 )
		printf("%s", "King");
	if ( x == 1 )
		printf("%s", "Ace");
}
//for qsort() needs to know how to deal with a struct
int compareCards( card_t** c1, card_t** c2) {
 if( (*c1)->number == (*c2)->number ) {
 return (*c1)->suit - (*c2)->suit;
 }
 return (*c1)->number - (*c2)->number;
}

int main () {
	int i,j; //for the loops
	card_t *deck[52]; //initialize the array for the deck of cards
	//populate deck with cards
	for( i = 0; i < 4; i++) {
		for( j = 0; j < 13; j++) {
			deck[i*13+j] = malloc(sizeof(card_t)); //add some space to the heap
			deck[i*13+j] ->number = 1+j;   //fancy "pointer to member operator"
			deck[i*13+j] ->suit = i;
		}
	}
	//outpus the created deck
        for ( i = 0; i < 52; i++){
		if (deck[i]->number == 11 || deck[i]->number== 12 || deck[i]->number== 13 || deck[i]->number== 1){
			 getNumber(deck[i]->number);
		}
		else 
			printf("%i", deck[i]->number);
		printf(" %s\n", getSuit(deck[i]->suit));

	}
	//the shuffling function
	int swapper = 0; //index of card to be swapped
        card_t *temp[1]; //temp holding place for swap
	temp[0] = malloc(sizeof(card_t));
        srand(time(NULL)); //seed the random numbers with current time
        for(i = 0; i < 52; i++){
        	//generate a pseudo-random number from 0 to 51
        	swapper = rand() % 52;
        	//swap current card with the swapper
       		temp[0] = deck[i];
        	deck[i] = deck[swapper];
         	deck[swapper] = temp[0];
	}
	//outputs the shuffled deck
       	printf("Now the Shuffled deck\n"); 
        for ( i = 0; i < 52; i++){
		if (deck[i]->number == 11 || deck[i]->number== 12 || deck[i]->number== 13 || deck[i]->number== 1){
			 getNumber(deck[i]->number);
		}
		else 
			printf("%i", deck[i]->number);
		printf(" %s\n", getSuit(deck[i]->suit));

	}
	//now resorts it and outputs it
	printf("Now the sorted\n");
	qsort(deck, 52, sizeof(card_t *), (int (*)(const void *,const void *)) compareCards );
        for ( i = 0; i < 52; i++){
		if (deck[i]->number == 11 || deck[i]->number== 12 || deck[i]->number== 13 || deck[i]->number== 1){
			 getNumber(deck[i]->number);
		}
		else 
			printf("%i", deck[i]->number);
		printf(" %s\n", getSuit(deck[i]->suit));
	}
	return 0;
}

