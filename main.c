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


//get an integer input from standard input (keyboard)
//return : input integer value
//         (-1 is returned if keyboard input was not integer)
int getIntegerInput(void) {
    int input, num;
    
    num = scanf("%d", &input);
    fflush(stdin);
    if (num != 1) //if it fails to get integer
        input = -1;
    
    return input;
}

//card processing functions ------------------------------------------------------------------------------------------------------------------------------------------------
//calculate the actual card number in the blackjack game
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
		cardnum = 10;

	return cardnum;
}
	
//print the card information (e.g. DiaA)
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
}

	
//card array controllers -------------------------------------------------------------------------------------------------------------------------------------- 
//mix the card sets and put in the array
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
 
}

//get one card from the tray
int pullCard(void) {
 
	int pull_card;  
	
	pull_card = CardTray[cardIndex];
	cardIndex++;
	
	if (cardIndex >= 48)
		gameEnd = 1;
	
	return pull_card;	
}


//playing game functions -----------------------------

//player settiing
int configUser(void) {

	do {
	printf("input the number of player(Max: %d): ", N_MAX_USER);
	n_user = getIntegerInput();
	if (N_MAX_USER < n_user)
		printf("\nToo Many Players !!\n");
	else if(n_user == -1)
		printf("Please again =_=;\n\n"); 		
	}while(N_MAX_USER < n_user || n_user == -1);
	
	return n_user;
}


//betting
void betDollar(void) {

	int i;
	
	printf("\n\n----------------BETTING STEP-----------------\n");
	do{
	if (bet[0] == -1)
		printf("Please again =_=;\n\n");
	printf("-> Your betting(total: %d): ", dollar[0]); //This is user's betting(Player 0) 
	bet[0] = getIntegerInput();		
	} while (bet[0] == -1);


	for (i=1; i<n_user; i++) // This is computer players 
	{
		bet[i] = 1+(rand()%N_MAX_BET);
		printf("-> player%d's betting: %d (out of %d) \n", i, bet[i], dollar[i]);
	}
}


//offering initial 2 cards
void offerCards(void) {
	int i;
	//1. give two card for each players
	for (i=0;i<n_user;i++)
	{
		cardhold[i][0] = pullCard();
		cardhold[i][1] = pullCard();
	}
	//2. give two card for the operator
	cardhold[n_user][0] = pullCard();
	cardhold[n_user][1] = pullCard();
	
	return;
}

//print initial card status
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

//Get action from players 
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

//Print  
void printUserCardStatus(int user, int cardcnt) {
	int i;
	
	printf("   -> card : ");
	for (i=0; i<cardcnt; i++)
		printCard(cardhold[user][i]);
	printf("\t ::: ");
}

// calculate the card sum and see if : 1. under 21, 2. over 21, 3. blackjack
void calcStepResult(int user, int cardcnt) {

	if (user < n_user)
	{
		if (cardSum[user] > 21)
		{
			printf("\t ::: DEAD (sum: %d)", cardSum[user]);
			dollar[user] -= bet[user];
			printf(" --> -$%d ($%d)", bet[user], dollar[user]);
		}
		else if (cardSum[user] == 21)
			{
				if (cardcnt == 2)
				{
					printf("\t ::: Black Jack Congratuation!");
					dollar[user] += (bet[user]*2); 
					cardSum[user] = 100;
				}
			}
		else 
			printf("\t ::: ");	
	}
	else
		{
		if(cardSum[user] > 21)
			{
			printf("\t ::: Dealer Dead (sum: %d)", cardSum[user]);
			} 
		else if(cardSum[user] == 21)
			{
				if (cardcnt == 2)
					{
					printf("\t ::: Black Jack!");
					cardSum[n_user] = 100;
					}
				else 
					printf("\t ::: ");
			}
		}
}

void checkResult(void) { 
	
	int i;

	for (i=0; i<n_user; i++)
	{
		if (cardSum[n_user] == 100)
		{
			if(cardSum[i] == 100)
			{
				if(i==0)
					printf("\n --> Your result : Black Jack! --> $ %d", dollar[i]);
				else
					printf("\n --> Player %d result : Black Jack! --> $ %d", i, dollar[i]);	
			}
			else
			{
				if(i==0)
				{
					dollar[i] -= bet[i];
					printf("\n --> Your result : lose (Dealer is Blackjack) --> $ %d", dollar[i]);							
				}
				else
				{
				dollar[i] -= bet[i];
				printf("\n --> Player %d result : lose (Dealer is Blackjack) --> $ %d", i, dollar[i]);						
				}
			}
		}
		else if (cardSum[i] > 21)
		{
			if(i==0)
				printf("\n --> Your result : lose due to overflow (sum : %d) --> $ %d", cardSum[i], dollar[i]);
			else
				printf("\n --> Player %d result : lose due to overflow (sum : %d)--> $ %d", i, cardSum[i], dollar[i]);	
		}
		else
		{	
			if(cardSum[n_user] <= 21)
			{
				if (cardSum[i] >= cardSum[n_user])
				{
					if(i==0)
					{
						dollar[i] += bet[i];
						printf("\n --> Your result : win (sum : %d) --> $ %d", cardSum[i], dollar[i]);					
					}
					else 
					{
						dollar[i] += bet[i];
						printf("\n --> Player %d result : win (sum : %d) --> $ %d", i, cardSum[i], dollar[i]);					
					}
				}
				else 
				{
					if(i==0)
					{
						dollar[i] -= bet[i];
						printf("\n --> Your result : lose (sum : %d) --> $ %d", cardSum[i], dollar[i]);					
					}
					else 
					{
						dollar[i] -= bet[i];
					printf("\n --> Player %d result : lose (sum : %d)--> $ %d", i, cardSum[i], dollar[i]);					
					}
				}
			}
			else
			{
				if(i==0)
				{
					if(cardSum[i] > 21)
						printf("\n --> Your result : lose due to overflow (sum : %d) --> $ %d", cardSum[i], dollar[i]);	
					else 
					{
						dollar[i] += bet[i];
						printf("\n --> Your result : win (sum : %d) --> $ %d", cardSum[i], dollar[i]);	
					}	
				}
				else 
				{
					if(cardSum[i] > 21)
						printf("\n --> Your result : lose due to overflow (sum : %d) --> $ %d", cardSum[i], dollar[i]);
					else			
					{
						dollar[i] += bet[i];
						printf("\n --> Player %d result : win (sum : %d) --> $ %d", i, cardSum[i], dollar[i]);	
					}	
				}
			}
		} 
	}
}

void checkWinner(void) {

		int max, i;
		int winner;

		
		for(i=0; i<n_user; i++)
		{
			if (i==0)
				printf("\nYou : %d", dollar[i]);
			else 
				printf("\nPlayer %d : %d", i, dollar[i]);	
		}
		
		max = dollar[0];		
		for(i=1; i<n_user; i++)
		{
			if (dollar[i] > max)
				{
				max = dollar[i];
				winner = i;
				}
		}
		
		if (winner == 0)
			printf("\n\nFinal winner is YOU !! => Eat chicken !!");
		else 
			printf("\n\nFinal Winner is player %d", winner);

}

int main(int argc, char *argv[]) {

	int roundIndex = 1;
	int max_user;
	int j, i;
	int action = 0;
	int cardcnt = 2;
	int flag = 0;
	srand((unsigned)time(NULL));
	
	//set the number of players
	max_user = configUser(); // 맞는지 확인하기 
	
	//Game initialization --------
	//1. players' dollar	
	for(i=0; i<max_user; i++)
	{
		dollar[i] = N_DOLLAR;
		cardSum[i]=0;
	}
	
	//2. card tray
	mixCardTray();

	//Game start --------
	do{
		for(i=0; i<max_user; i++)
		{
			cardSum[i]=0;
		}
	
		printf("\n---------------------------------------------------------------\n");
		printf("-----------------------ROUND %d (cardIndex: %d)-----------------------", roundIndex, cardIndex);
		printf("\n----------------------------------------------------------------\n");
		
		betDollar();
		offerCards(); //1. give cards to all the players
		
		printf("\n---------------------CARD OFFERING--------------------------------\n");
		printCardInitialStatus();

		printf("\n---------------------- GAME start -------------------------------\n");
		
		//each player turn
		for (i=0; i<=n_user; i++)
		{
			action = 0;
			cardcnt = 2;
		
			if (i==0)
				printf("\n >>>> My turn!---------------------------------------\n");
			else if (i == n_user)
				printf("\n\n >>>> Dealer turn!-----------------------------------\n");
			else
				printf("\n >>>> Player %d turn!!-------------------------------\n", i);																						
			
			printUserCardStatus(i, cardcnt);		
			
			for(j=0; j<cardcnt; j++)
			{
				cardSum[i] += getCardNum(cardhold[i][j], i);
				if (getCardNum(cardhold[i][j], i) == 1)
					flag = 1;
			}	
			
			if(flag == 1 && cardSum[i] == 11)
			{	
				cardSum[i] = 21;	
				calcStepResult(i, cardcnt);
				action = 1;
			}
			else 
				calcStepResult(i, cardcnt);			
			
			while (action == 0)
			{	
				action = getAction(i);
				
				if (action != 0 )
					break;					
				
				cardhold[i][cardcnt] = pullCard();
			
				cardSum[i]=0;
				flag=0;	
				
				for(j=0; j<=cardcnt; j++)
				{
					cardSum[i] += getCardNum(cardhold[i][j], i);
					if (getCardNum(cardhold[i][j], i) == 1)
						flag = 1;
				}	
				
				if(flag == 1 && cardSum[i] <= 11)
				{
					cardSum[i] += 10;	
				}	
				cardcnt++;
			
				printUserCardStatus(i, cardcnt);				
				calcStepResult(i, cardcnt);				
				
				if (cardSum[i] >= 21)
					break;	
			}
		}
			
		//result
		printf("\n\n-----------------------------ROUND %d result-----------------------------------\n\n", roundIndex);
		checkResult();	

				//Game end 
		for (i=0; i<n_user; i++)
		{
			if (dollar[i] == 0)
			{
				gameEnd =1;
			}
		}
		
		if (cardIndex == 51)
			gameEnd == 1;
		
		roundIndex++; //Round 세기 
	
	} while (gameEnd == 0);
	
	//Game end
	printf("\n\n-----------------------------------GAME END----------------------------------------------\n");

	// Check the winner	
	checkWinner();
	
	printf("\nProgressed Gound: %d", roundIndex-1);

	return 0;
}
