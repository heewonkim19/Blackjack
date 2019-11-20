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
int cardIndex = 0; 							//Variable to indicate using the mixed card							

//Player info
int dollar[N_MAX_USER];						//dollars that each player has
int n_user;									//number of users

//Play yard information
int cardhold[N_MAX_USER+1][N_MAX_CARDHOLD];	//cards that currently the players hold
int cardSum[N_MAX_USER];					//sum of the cards
int bet[N_MAX_USER];						//current betting 
int gameEnd = 0; 							//game end flag


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

//Card processing functions ------------------------------------------------------------------------------------------------------------------------------------------------

//Calculate the actual card number in the blackjack game
int getCardNum(int cardnum, int user) {
	 
	if (cardnum <= 9)
		cardnum += 1; 
	else if ((cardnum >= 13) && (cardnum <= 22)) 
		cardnum %= 12;
	else if ((cardnum >= 26) && (cardnum <= 35)) 
		cardnum %= 25;
	else if ((cardnum >= 39) && (cardnum <= 48))
		cardnum %= 38;
	else 
		cardnum = 10; 						//Jack, Queen, King => 10

	return cardnum; 						//This is a actual number
}
	
//Print the card information (e.g. DiaA)
void printCard(int cardnum) {

	//From 0 to 12 => Hart, From 13 to 25 => Dia, From 26 to 38 => Spade, From 39 to 51 => Cluber 
	switch (cardnum)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		if (cardnum == 0)
			printf(" Hart A ");
		else if (cardnum == 10)
			printf (" Hart J ");
		else if (cardnum == 11)
			printf(" Hart Q ");
		else if (cardnum == 12)
			printf(" Hart K ");	
		else	
			printf(" Hart %d ",cardnum+1);
		break;
		
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		if (cardnum == 13)
			printf(" Dia A ");
		else if (cardnum == 23)
			printf(" Dia J ");
		else if (cardnum == 24)
			printf(" Dia Q ");
		else if (cardnum == 25)
			printf(" Dia K ");
		else
			printf(" Dia %d ",cardnum%12);
		break;	
		
		case 26:
		case 27:
		case 28:
		case 29:
		case 30:
		case 31:
		case 32:
		case 33:
		case 34:
		case 35:
		case 36:
		case 37:
		case 38:
		if (cardnum==26)
			printf(" Spade A ");
		else if (cardnum == 36)
			printf(" Spade J ");
		else if (cardnum == 37)
			printf(" Spade Q ");
		else if (cardnum == 38)
			printf(" Spade K ");
		else
			printf(" Spade %d ", cardnum%25);
		break;
		
		case 39:
		case 40:
		case 41:
		case 42:
		case 43:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		if (cardnum == 39)
			printf(" Club A ");
		else if (cardnum == 49)
			printf(" Club J ");
		else if (cardnum == 50)
			printf(" Club Q ");
		else if (cardnum == 51)
			printf(" Club K ");
		else
			printf(" Club %d ", cardnum%38);
		break;	
		
		}	

	return;
}
	
//Card array controllers -------------------------------------------------------------------------------------------------------------------------------------- 

//Mix the card sets and put in the array
void mixCardTray(void) {

	int i, j;
	int temp;
	
	//Initialize cardTray from 0 to 51
	for (i=0; i<N_CARDSET*N_CARD; i++)
	{
		CardTray[i]=i;
	}
	
	//Mix the number in the cardTray
	for (j=0; j<N_CARDSET*N_CARD; j++)
	{
		int k = rand()%(N_CARDSET*N_CARD);
		temp = CardTray[j];
		CardTray[j] = CardTray[k];
		CardTray[k] = temp;
	}

	return;
}

//Pull one card from the tray
int pullCard(void) {
 
	int pull_card;					 //Number pulled from the card tray  
	
	pull_card = CardTray[cardIndex]; //Pull out the card on the marked part
	cardIndex++;
	
	if (cardIndex >= 48) 			// If the card tray lacks cards, end the game 
		gameEnd = 2;
	
	return pull_card;	
}


//Playing game functions -----------------------------

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

	// Check the winner	
	checkWinner();
	
	//Print caunting round
	printf("\nProgressed Gound: %d", roundIndex-1);

	return 0;
}
