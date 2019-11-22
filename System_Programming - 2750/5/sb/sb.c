//Ervin Hennrich
//cs2750


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

//same as shuffle, using same struct and enum
typedef enum { clubs, diamonds, hearts, spades} suit_t;
typedef struct
{
unsigned int number : 4;
suit_t suit : 2;
} card_t;

//simple card comparison, needed for qsort() so it can sort through the struct
int compareCards( card_t** c1, card_t** c2) {
 if( (*c1)->suit == (*c2)->suit ) {
 return (*c1)->number - (*c2)->number;
 }
 return (*c1)->suit - (*c2)->suit;
}

//so i can output the words instead of just the number
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

//pretty simple, returns the suit.
char * getSuit(int suit_t) {
	switch(suit_t) {
		case diamonds: return "D";
		case hearts: return "H";
		case clubs: return "C";
		case spades: return "S";
	}
	return NULL;
}

//this uses a bitwise inlcusive or statment to change the bit $in places
void changebit(char *arr, int in) {
    arr[in / 8] |= 1 << (in % 8);
}

//returns the bit, checks the bit and compares it to 1
char getdabit(char *arr, int in) {
    return 1 & (arr[in / 8] >> (in % 8));
}


int main(){
	int i,j; //for loops
	int size = 7; //makes for an array with 56 bits
	card_t *deck[52];//initiaze the deck
	
	for( i = 0; i < 4; i++) {
		for( j = 0; j < 13; j++) {
			deck[i*13+j] = malloc(sizeof(card_t)); //add some space to the heap for each of the elements the size of the struct
			deck[i*13+j] ->number = 1+j;   //fancy "pointer to member operator" counts up the cards in each suit
			deck[i*13+j] ->suit = i; 
		}
	}

	char A[size]; //for bit vector
	memset(A, 0, 8*sizeof(char)); //to add space and prepare for population
	int go = 0; //switch to make sure when checking array does a full sweep of whole array if it cant find anything its first go aground
	int swapper = 0; //index of card to be swapped
	card_t *temp[1]; //temp holding place for swap
	temp[0] = malloc(sizeof(card_t)); //prepare memory for temp to hold cad_t
        srand(time(NULL)); //seed the random numbers with current time
        for(i = 0; i < 52; i++){
        	//generate a random number from 0 to 51
        	swapper = rand() % 52;
		if (getdabit(A,swapper) == 0){
        	//swap current card with the swapper
       			temp[0] = deck[i];
        		deck[i] = deck[swapper];
         		deck[swapper] = temp[0];
			//change bit
			changebit(A, swapper);
		}
		else {
			for(j = swapper; j<52; j++){ //loop to find the next available 0
				if (getdabit(A,j) == 0){
					temp[0] = deck[i];//this part was confusing, stores the card to be swapped
					deck[i] = deck[j]; //switches with j (the index of available 0 bit)
					deck[j] = temp[0]; //and deck[j] now gets deck[i] 
					changebit(A, j); // then change the corresponding bit j
					break;
				 }
				if (go == 0 && j == 51){//if it cant find anything from original rand onwards does another pass
					go = 1;
					j=0;
				}
			}
		}
	}
		 //this prints the shuffled array, I know it should be its own function
        for ( i = 0; i < 52; i++){
		if (deck[i]->number == 11 || deck[i]->number== 12 || deck[i]->number== 13 || deck[i]->number== 1){
			 getNumber(deck[i]->number);
		}
		else 
			printf("%i", deck[i]->number);
		printf(" %s\n", getSuit(deck[i]->suit));
	}
		//does the same qsort as in shuffle, and prints out the deck in order
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
