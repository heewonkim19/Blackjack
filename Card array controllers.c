/*-----------------------------------------------------------------------------------------Card array controllers--------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

#define N_CARDSET			1
#define N_CARD				52

//Card tray object
int CardTray[N_CARDSET*N_CARD]; 			//Card Tray
int cardIndex = 0; 							//Variable to indicate using the mixed card							

//Play yard information
int gameEnd = 0; 							//game end flag

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
