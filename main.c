/*BLACKJACK GAME*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include "Card array controllers.c"
#include "Card processing functions.c"
#include "Playing game functions.c"


#define N_CARDSET			1
#define N_CARD				52
#define N_DOLLAR			50


#define N_MAX_CARDNUM		13
#define N_MAX_USER			5
#define N_MAX_CARDHOLD		10
#define N_MAX_GO			17
#define N_MAX_BET			5

#define N_MIN_ENDCARD		30


//Card tray object
extern int CardTray[N_CARDSET*N_CARD]; 			//Card Tray
extern int cardIndex; 							//Variable to indicate using the mixed card							

//Player information
extern int dollar[N_MAX_USER];						//dollars that each player has
extern int n_user;									//number of users

//Play yard information
extern int cardhold[N_MAX_USER+1][N_MAX_CARDHOLD];	//cards that currently the players hold
extern int cardSum[N_MAX_USER];					//sum of the cards
extern int bet[N_MAX_USER];						//current betting 
extern int gameEnd; 							//game end flag


//Card processing functions ------------------------------------------------------------------------------------------------------------------------------------------------
//Calculate the actual card number in the blackjack game
extern int getCardNum();	

//Print the card information (e.g. DiaA)
extern void printCard();

	
//Card array controllers -------------------------------------------------------------------------------------------------------------------------------------- 
//Mix the card sets and put in the array
extern void mixCardTray();

//Pull one card from the tray
extern int pullCard();


//Playing game functions -------------------------------------------------------------------------------------------------------------------------------------
//Player settiing
extern int configUser(); 

//Bet the Dollar
extern void betDollar();

//Offering initial 2 cards
extern void offerCards(); 

//Print initial card status
extern void printCardInitialStatus();																		

//Get action from players 
extern int getAction();

//Print card status each player and dealer
extern void printUserCardStatus();

//Calculate the card sum and see if : 1. under 21, 2. over 21, 3. blackjack
extern void calcStepResult();

//Print the results after each round
extern void checkResult();

//Print who winner is after game (The player with the most money becomes the winner)
extern void checkWinner();


//Get an integer input from standard input (keyboard)
//Return : input integer value
//         (-1 is returned if keyboard input was not integer)
int getIntegerInput(void) {
    int input, num;
    
    num = scanf("%d", &input);
    fflush(stdin);
    if (num != 1) 							//if it fails to get integer
        input = -1;
    
    return input;
}


int main(int argc, char *argv[]) {

	int roundIndex = 1; 			//Counting round
	int max_user; 					//The number of player in the game
	int i, j; 						//Variable for <for sentence>
	int action = 0; 				//Go? Stop?
	int cardcnt = 2; 				//How many times pull the card
	int flag = 0; 					//Variable flag for A=1? A=11?
	
	srand((unsigned)time(NULL)); 	//Initialize rand()
	
	//set the number of players
	max_user = configUser(); 
	
	//Game initialization -----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	//1. Initialize players' dollar
	for(i=0; i<max_user; i++)
	{
		dollar[i] = N_DOLLAR;
	}
	
	//2. card tray
	mixCardTray(); 					//Mix card of card tray

	
	//Game start --------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	do{
		for(i=0; i<max_user; i++) 	// Initialize player's cardSum
		{
			cardSum[i]=0;
		}
	
		printf("\n---------------------------------------------------------------\n");
		printf("-----------------------ROUND %d (cardIndex: %d)-----------------------", roundIndex, cardIndex);
		printf("\n----------------------------------------------------------------\n");
		
		betDollar(); 				//bets the dollar
		offerCards(); 				//give cards to all the players
		
		printf("\n---------------------CARD OFFERING--------------------------------\n");
		printCardInitialStatus(); 	//print card initial status

		printf("\n---------------------- GAME start -------------------------------\n");
		
		//each player turn
		for (i=0; i<=n_user; i++)
		{
			action = 0; 			//Initialize variable
			cardcnt = 2; 			//Initialize variable
		
			if (i==0) 				//User
				printf("\n >>>> My turn!---------------------------------------\n");
			else if (i == n_user) 	//Dealer
				printf("\n\n >>>> Dealer turn!-----------------------------------\n");
			else 					//Players
				printf("\n >>>> Player %d turn!!-------------------------------\n", i);																						
			
			printUserCardStatus(i, cardcnt); //Print user status 	
					
			while (action == 0)
			{	
				cardSum[i]=0;												//Initialize cardSum
				flag=0;														//Initialize flag		
				for(j=0; j<cardcnt; j++)
				{
					cardSum[i] += getCardNum(cardhold[i][j], i); 			//Sum of cardhold
					if (getCardNum(cardhold[i][j], i) == 1)					//If player or dealer has 'Ace'
						flag = 1;
				}	
				if (flag == 1 && cardSum[i]==11)
				{
					cardSum[i] = 21;
					calcStepResult(i, cardcnt);
					break;
				}
				
				if(flag == 1 && cardSum[i] <= 11) 							//If sum of cards is less than 11, 'Ace' is 11
				{
					cardSum[i] += 10;	
				}
				
				action = getAction(i); 										//Go? Stop?
				if (action != 0)
					break;
			
				cardhold[i][cardcnt] = pullCard(); 							//Pull card and Put to cardhold
			
			 	cardcnt++;													//cardcnt +1 after pulling card 
				
				cardSum[i]=0;												//Initialize cardSum
				flag=0;														//Initialize flag		
				for(j=0; j<cardcnt; j++)
				{
					cardSum[i] += getCardNum(cardhold[i][j], i); 			//Sum of cardhold
					if (getCardNum(cardhold[i][j], i) == 1)					//If player or dealer has 'Ace'
						flag = 1;
				}	
				
				if(flag == 1 && cardSum[i] <= 11) 							//If sum of cards is less than 11, 'Ace' is 11
				{
					cardSum[i] += 10;	
				}	
 			
				printUserCardStatus(i, cardcnt); 							//Print user card status			
				calcStepResult(i, cardcnt);									//Calculate step result		
				
				if (cardSum[i] > 21) 										//If cardSum is more than 21, go out 'while'
					break;	
			}
		}
			
		//Result
		printf("\n\n-----------------------------ROUND %d result-----------------------------------\n\n", roundIndex);
		checkResult();	

		//If someone's dollar is zero, game over 
		for (i=0; i<n_user; i++)
		{
			if (dollar[i] == 0)
			{
				gameEnd =1;
			}
		}
		
		roundIndex++; 													//roundIndex +1 after each round 
		
		if (gameEnd == 2)
			printf("\n\n--------------------------------------cardIndex = %d---------------------------------------------", cardIndex); //print cardIndex

	} while (gameEnd == 0);
	
	//Game end
	printf("\n\n-----------------------------------GAME END----------------------------------------------\n");

	//Check the winner	
	checkWinner();
	
	//Print caunting round
	printf("\nProgressed Gound: %d", roundIndex-1);

	return 0;
}
