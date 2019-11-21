/*----------------------------------------------------------------------------------Playing game functions--------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

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
int CardTray[N_CARDSET*N_CARD]; 			//Card Tray
						
//Player information
int dollar[N_MAX_USER];						//dollars that each player has
int n_user;									//number of users

//Play yard information
int cardhold[N_MAX_USER+1][N_MAX_CARDHOLD];	//cards that currently the players hold
int cardSum[N_MAX_USER];					//sum of the cards
int bet[N_MAX_USER];						//current betting 

extern void printCard();
extern int getIntegerInput();


//Player settiing
int configUser(void) {

	do {
	printf("input the number of player(Max: %d): ", N_MAX_USER);
	n_user = getIntegerInput();
	if (N_MAX_USER < n_user) 					//If N_MAX_USER < n_user, print
		printf("\nToo Many Players !!\n");
	else if(n_user == -1) 						//If the value entered is not an integer, print 
		printf("Please again =_=;\n\n"); 		
	}while(N_MAX_USER < n_user || n_user == -1); //N_MAX_USER < n_user OR n_user = not an integer => Repetition
	
	return n_user;
}


//Bet the Dollar
void betDollar(void) {

	int i;
	
	printf("\n\n------------------BETTING STEP---------------------\n");
	do{
	printf("-> Your betting(total: %d): ", dollar[0]); 								//This is user's betting(Player 0) 
	bet[0] = getIntegerInput();
	
	if (bet[0] == -1) 																//If the value entered is not an integer, print
	printf("Please again =_=;\n\n");		
	} while (bet[0] == -1); 														//If the value entered is not an integer => Repetition


	for (i=1; i<n_user; i++) 														//This is computer players 
	{
		bet[i] = 1+(rand()%N_MAX_BET); 												//Batting at random number from 1 to 5
		printf("-> player%d's betting: %d (out of %d) \n", i, bet[i], dollar[i]); 	//Show betting money of Each player
	}
	
	return;
}


//Offering initial 2 cards
void offerCards(void) {
	
	int i;
	
	//1. give two card for each player
	for (i=0;i<n_user;i++)
	{
		cardhold[i][0] = pullCard();
		cardhold[i][1] = pullCard();
	}
	
	//2. give two card for the dealer
	cardhold[n_user][0] = pullCard();
	cardhold[n_user][1] = pullCard();
	
	return;
}

//Print initial card status
void printCardInitialStatus(void) {
   
   int i;

    printf("-> dealer: X ");				 //First card of dealer not show
	printCard(cardhold[n_user][1]); 		//Second card of dealer
	
	//Print initial user card status   
	printf("\n-> you: "); 
	printCard(cardhold[0][0]);
	printCard(cardhold[0][1]);
   
   	//print initial computer players status
	for(i=1; i<n_user-1; i++)
	{
	printf("\n-> player%d: ",i);
    printCard(cardhold[i][0]);
    printCard(cardhold[i][1]);
	}
	
   return;
}

//Get action from players 
int getAction(int user){
	
	int action =-1; 
	
	if (user == 0)							 //Ask to user
	{
		while(action == -1) 
		{
			printf("\t ::: Action?(0-Go, other integer-Stop)");
			action = getIntegerInput();
		}
	}
	else 									//Ask to computer players and dealer
	{
		if (cardSum[user] < 17)				 //If the card is less than 17, Go
		{
			printf("\t ::: GO!");
			printf("\n");			
			action = 0;
		}
		else								 //If sum of cards is over 17, Stop
		{
			printf("\t ::: STAY!");
			printf("\n");
			action = 1;
		}
	}
	return action;  
}

//Print card status each player and dealer
void printUserCardStatus(int user, int cardcnt) {
	int i;
	
	printf("   -> card : ");
	for (i=0; i<cardcnt; i++)
		printCard(cardhold[user][i]); 		//Print pulled cards
	printf("\t ::: ");
}

//Calculate the card sum and see if : 1. under 21, 2. over 21, 3. blackjack
void calcStepResult(int user, int cardcnt) {

	if (user < n_user) //Each player 
	{
		if (cardSum[user] > 21) 										//If sum of cards is over than 21, Dead
		{
			printf("\t ::: DEAD (sum: %d)", cardSum[user]);
			dollar[user] -= bet[user]; 									//Subtract the betting amount from dollars that each player has 
			printf(" --> -$%d ($%d)", bet[user], dollar[user]);			 // Print betting money and the dollar left
		}
		else if (cardSum[user] == 21)									 //If sum of player's cards is 21
			{
				if (cardcnt == 2) 										//If player is Blackjack, Print and Get double the amount of betting and Change cardSum to 100 (Because the sum of the cards will never be 100)
				{
					printf("\t ::: Black Jack Congratuation!");
					dollar[user] += (bet[user]*2); 
					cardSum[user] = 100;
				}
			}
		else 															//The sum of the cards is 21, but not blackjack
			printf("\t ::: ");	
	}
	else
		{
		if(cardSum[user] > 21)											 //If sum of player's cards is over than 21, Dealer is dead 
			{
			printf("\t ::: Dealer Dead (sum: %d)", cardSum[user]);
			} 
		else if(cardSum[user] == 21)									 //If sum of dealer's cards is 21 
			{
				if (cardcnt == 2)										 //If dealer is Blackjack, Print and Change cardSum to 100 (Because the sum of the cards will never be 100)  
					{
					printf("\t ::: Black Jack!");
					cardSum[n_user] = 100;
					}
				else 
					printf("\t ::: ");
			}
		}
	return;
}

//Print the results after each round
void checkResult(void) { 
	
	int i;
	
	for (i=0; i< n_user; i++)
	{
		if (cardSum[n_user] == 100)												//If dealer is Blackjack
		{
			if (cardSum[i] != 100)												//If player is not Blackjack
			{
				if (i==0)														//User
				{
				dollar[i] -= bet[i];											//Subtract the betting amount from dollars that each player has
				printf("\n --> Your result : lose (Dealer is Blackjack) --> $ %d", dollar[i]);
				}
				else 															//Computer player
				{	
				dollar[i] -= bet[i];											//Subtract the betting amount from dollars that each player has
				printf("\n --> Player %d result : lose (Dealer is Blackjack) --> $ %d", i, dollar[i]);		
				}			
			}
			else
			{
				if (i==0) 														//User
				{
				printf("\n --> Your result : win --> $ %d", dollar[i]);
				}
				else 															//Computer player
				{
				printf("\n --> Player %d result : win --> $ %d", i, dollar[i]);		
				}					
			} 
				
		}
		else 																	//If the dealer's not blackjack
		{
			if (i==0)															//User
			{
				if (cardSum[i] == 100) 											//If player is Blackjack
				{
					printf("\n --> Your result : Black Jack! --> $ %d", dollar[i]);				
				}
				else if (cardSum[i] > 21)										//If sum of user's cards is bigger than 21 
				{
					printf("\n --> Your result : lose due to overflow (sum : %d) --> $ %d", cardSum[i], dollar[i]);
				}	
				else if (cardSum[i] <= 21)										//If sum of user's cards is less than 21			
				{
					if (cardSum[n_user] > 21 || cardSum[i] >= cardSum[n_user]) //If sum of dealer's cards bigger than 21 OR If sum of user's cards is more than sum of dealer's cards
					{
						dollar[i] += bet[i];									//Get the amount of betting
						printf("\n --> Your result : win (sum : %d) --> $ %d", cardSum[i], dollar[i]);
					}
					else if (cardSum[i] < cardSum[n_user]) 						//If sum of dealer's cards is bigger than sum of user's cards
					{
						dollar[i] -= bet[i];									//Subtract the betting amount from dollars that user has 
						printf("\n --> Your result : lose (sum : %d) --> $ %d", cardSum[i], dollar[i]);
					}
				}	
			
			}
			else																//Computer player
			{
				if (cardSum[i] == 100)											 //If player is not Blackjack
				{
					printf("\n --> Player %d result : Black Jack! --> $ %d", i, dollar[i]);	
				}
				else if (cardSum[i] > 21) 										// If sum of player's cards is bigger than 21
				{
					printf("\n --> Player %d result : lose due to overflow (sum : %d)--> $ %d", i, cardSum[i], dollar[i]);
				}
				else if (cardSum[i] <= 21)										//If sum of player's cards is less than 21
				{
					if (cardSum[n_user] > 21 || cardSum[i] >= cardSum[n_user]) //If sum of dealer's cards bigger than 21 OR If sum of player's cards is more than sum of dealer's cards
					{
						dollar[i] += bet[i];									//Get the amount of betting
						printf("\n --> Player %d result : win (sum : %d) --> $ %d", i, cardSum[i], dollar[i]);
					}
					else if (cardSum[i] < cardSum[n_user])						//If sum of dealer's cards is bigger than sum of player's cards
					{
						dollar[i] -= bet[i];									//Subtract the betting amount from dollars that user has 
						printf("\n --> Player %d result : lose (sum : %d)--> $ %d", i, cardSum[i], dollar[i]);
					}
				}		
			}
		}	
	}
	return;
}

//Print who winner is after game (The player with the most money becomes the winner)
void checkWinner(void) {

		int max, i;
		int winner;

		for(i=0; i<n_user; i++) 										//Print dollars that each player has
		{ 
			if (i==0) 													//User
				printf("\nYou : %d", dollar[i]);
			else 														//Computer players
				printf("\nPlayer %d : %d", i, dollar[i]);	
		}
		
		max = dollar[0]; 												//Assume that user have the largest amount of money		
		for(i=1; i<n_user; i++)
		{
			if (dollar[i] > max) 										//If dollar[i] is bigger than max
				{
				max = dollar[i]; 										//Initialize max to dollar[i]
				winner = i; 											//Put i in winner
				}
		}
		
		if (winner == 0) 												//If winner is user
			printf("\n\nFinal winner is YOU !! => Eat chicken !!");
		else															 //If winner is player(?)
			printf("\n\nFinal Winner is player %d", winner);

	return;
}
