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

//card tray object
int CardTray[N_CARDSET*N_CARD];
int cardIndex = 0;							


//player info
int dollar[N_MAX_USER];						//dollars that each player has
int n_user;									//number of users


//play yard information
int cardhold[N_MAX_USER+1][N_MAX_CARDHOLD];	//cards that currently the players hold
int cardSum[N_MAX_USER];					//sum of the cards
int bet[N_MAX_USER];						//current betting 
int gameEnd = 0; 							//game end flag

//Card processing functions -------------------------------------------------------------------------------

//Calculate the actual card number in the blackjack game
int getCardNum(int cardnum, int user) {
	
	int input;
	
	if (cardnum <= 9)
		input = cardnum + 1;
	else if (cardnum >= 13 && cardnum <= 22)
		input = cardnum % 12;
	else if (cardnum >= 26 && cardnum <= 35)
		input = cardnum % 25;
	else if (cardnum >= 39 && cardnum <= 48)
		input = cardnum %38;
	else 
		input = 10;
		
	if (input == 1)
	{
		if(cardSum[user] <= 10)
			input = 11;
		else 
			input = 1;
	}
	
	return input;

}

//Print the card information (e.g. DiaA)
void printCard(int cardnum) {
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
		if (cardnum==0)
			printf(" Hart A ");
		else if (cardnum==10)
			printf (" Hart J ");
		else if (cardnum==11)
			printf(" Hart Q ");
		else if (cardnum==12)
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
		if (cardnum==13)
			printf(" Dia A ");
		else if (cardnum==23)
			printf(" Dia J ");
		else if (cardnum==24)
			printf(" Dia Q ");
		else if (cardnum==25)
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
		else if (cardnum==36)
			printf(" Spade J ");
		else if (cardnum==37)
			printf(" Spade Q ");
		else if (cardnum==38)
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
		if (cardnum==39)
			printf(" Club A ");
		else if (cardnum==49)
			printf(" Club J ");
		else if (cardnum==50)
			printf(" Club Q ");
		else if (cardnum==51)
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
	
	for (i=0; i<N_CARDSET*N_CARD; i++)
	{
		CardTray[i]=i;
	}
	
	for (j=0; j<N_CARDSET*N_CARD; j++)
	{
		int k = rand()%(N_CARDSET*N_CARD);
		temp = CardTray[j];
		CardTray[j] = CardTray[k];
		CardTray[k] = temp;
	}
	
	return;
}

//Get one card from the tray
int pullCard(void) {
 
	int pull_card;  
	
	pull_card = CardTray[cardIndex];
	cardIndex++;
	
	if (cardIndex >= 48)
		gameEnd = 1;
	
	return pull_card;	
}

//Playing game functions -------------------------------------------------------------------------------------------------------------------------------------------------------------------

//Player settiing
int configUser(void) {

	do {
	printf("input the number of player(Max:5): ");
	scanf("%d", &n_user);
	
	if (N_MAX_USER < n_user)
		printf("\nToo Many Players !!\n");
		
	}while(N_MAX_USER < n_user);
	
	return n_user;
}

//Betting
void betDollar(void) {

	int i;
	
	printf("\n\n-----------------BETTING STEP---------------------\n");
	
	printf(" -> Your betting(total: %d): ", dollar[0]); //This is user's betting (Player 0) 
	scanf("%d", &bet[0]);

	for (i=1; i<n_user; i++) // This is computer players 
	{
		bet[i] = 1+(rand()%N_MAX_BET);
		printf("\n -> player%d's betting: %d (out of %d)\n", i, bet[i], dollar[i]);
	}
	
	return; 
}

//Offering initial 2 cards
void offerCards(void) {

	int i;
	
	//1. Give two card for each players
	for (i=0;i<n_user;i++)
	{
		cardhold[i][0] = pullCard();
		cardhold[i][1] = pullCard();
	}

	//2. Give two card for the operator
	cardhold[n_user][0] = pullCard();
	cardhold[n_user][1] = pullCard();
	
	return;
}

//Print initial card status
void printCardInitialStatus(void) {
   
   int i;

    printf("-> dealer: X ");
	printCard(cardhold[n_user][1]);

	printf("\n-> you: ");
	printCard(cardhold[0][0]);
	printCard(cardhold[0][1]);
   
	for(i=1; i<n_user-1; i++)
	{
	printf("\n-> player%d: ",i);
    printCard(cardhold[i][0]);
    printCard(cardhold[i][1]);
	}
	
   return;
}

//Ask "Go" or "Stop" to player
int getAction(int user){
	
	int action;
	
	if (user == 0)
	{
		printf("\t ::: Action?(0-Go, other integer-Stop)");
		scanf("%d", &action);
	}
	else 
	{
		if (cardSum[user] < 17)
		{
			printf("\t ::: GO!");
			printf("\n");			
			action = 0;
		}
		else
		{
			printf("\t ::: STAY!");
			printf("\n");
			action = 1;
		}
	}

	return action;  
}

//Print user's card status
void printUserCardStatus(int user, int cardcnt) {
	
	int i;
	
	printf("   -> card : ");
	for (i=0; i<cardcnt; i++)
		printCard(cardhold[user][i]);
	printf("\t ::: ");
	
	return;
}


int main(int argc, char *argv[]) {
	
	return 0;
}
