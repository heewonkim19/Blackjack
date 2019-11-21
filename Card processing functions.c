/*------------------------------------------------------------------------------- Card processing functions---------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>


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
	
