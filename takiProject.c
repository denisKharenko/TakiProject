#define _CRT_SECURE_NO_WARNINGS								// Denis Kharenko 324464536
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h> // in order to use the "rand" and "srand" functions
#include <time.h>	// in order to use "time" function

#define CARD_NUMBER_1 1
#define CARD_NUMBER_2 2
#define CARD_NUMBER_3 3
#define CARD_NUMBER_4 4
#define CARD_NUMBER_5 5
#define CARD_NUMBER_6 6
#define CARD_NUMBER_7 7
#define CARD_NUMBER_8 8
#define CARD_NUMBER_9 9
#define PLUS_CARD_NUMBER 10
#define STOP_CARD_NUMBER 11
#define CHANGE_DIRECTION_CARD_NUMBER 12
#define TAKI_CARD_NUMBER 13
#define CHANGE_COLOR_CARD_NUMBER 14
#define YELLOW_COLOR 'Y'
#define YELLOW_COLOR_NUMBER 1
#define RED_COLOR 'R'
#define RED_COLOR_NUMBER 2
#define BLUE_COLOR 'B'
#define BLUE_COLOR_NUMBER 3
#define GREEN_COLOR 'G'
#define GREEN_COLOR_NUMBER 4
#define NO_COLOR ' '
#define NO_COLOR_CARD_NUMBER 5
#define PLUS_CARD "+"
#define STOP_CARD "STOP"
#define CHANGE_DIRECTION_CARD "<->"
#define TAKI_CARD "TAKI"
#define CHANGE_COLOR_CARD "COLOR"
#define AMOUNT_OF_COLORS 4
#define INITIAL_CARDS_AMOUNT 4
#define REGULAR_CARDS 9
#define ALL_CARD_TYPES 14
#define START_PRINTING_IN_COLOR true
#define FINISH_PRINTING_IN_COLOR false
#define TAKE_CARD_ACTION 0
#define FINISH_TURN_ACTION 0
#define FORWARD_DIRECTION '+'
#define BACKWARDS_DIRECTION '-'
#define STOP_CARD_ADVANCEMENT 2
#define REGULAR_ADVANCEMENT 1

typedef struct card {  // a type to store card details
	int number;
	char color;

}CARD;	// a shorter name for the struct

typedef struct player {  // a type to store player details
	char playerName[21];
	CARD* cardDeck;
	int cardDeckLogicSize;
	int cardDeckSize;

}PLAYER;	// a shorter name for the struct

typedef struct cardStatistic {	// a type to store card statistic details
	int cardNumber;
	int apearanceAmount;
}CARD_STATISTIC;

//function declarations: (documentation for each function is available at its defininion below the main)

void printStartingMessage();
void checkDynamicAllocation(void* ptr);
void intializeStatisticsArray(CARD_STATISTIC cardsStatistics[]);
void playGame(PLAYER* playersData, int amountOfPlayers, CARD upperCard, CARD_STATISTIC cardsStatistics[]);
void printCard(CARD cardToPrint);
void printSpecialCardType(CARD cardToPrint);
void printPlayerCards(PLAYER player);
void gameOptionValidation(PLAYER player, int* gameOption, int specialCard);
void changeColorActions(CARD* upperCard);
void printColorOptions();
void changeDirectionCardActions(char* direction);
void mergeSortForStatistics(CARD_STATISTIC cardsStatistics[], int size);
void copyArray(CARD_STATISTIC cardsStatistics[],  int size, CARD_STATISTIC temporaryCardsStatistics[]);
void printCardStatistics(CARD_STATISTIC cardsStatistics[], int size);
int recieveAndCheckPlayerAmount();
int gameOptionsMenuDisplay(PLAYER player, int specialCard);
int advanceTurn(int specialCard, int currentPlayer, int amountOfPlayers, char* direction);
int stopCardActions(int currentPlayer, int amountOfPlayers, char direction);
int calculateNextPlayerNumber(int currentPlayer, int amountOfPlayers, char direction, int advancement);
PLAYER* preGamePreparations(int* amountOfPlayers, CARD_STATISTIC cardsStatistics[], CARD* upperCard);
PLAYER* receivePlayersData(int players);
PLAYER* intializeGame(int players, PLAYER* playersData, CARD* upperCard, CARD_STATISTIC cardsStatistics[]);
PLAYER* playTurn(PLAYER* playersData, int currentPlayer, CARD* upperCard, int* specialCard , CARD_STATISTIC cardsStatistics[]);
PLAYER* freeAllMemory(PLAYER* playersData, int amountOfPlayers);
char numberToCharConversion(int number);
CARD generateCard(int cardNumberToGenerateUpTo, CARD_STATISTIC cardsStatistics[]);
PLAYER generateInitialCards(PLAYER player, CARD_STATISTIC cardsStatistics[]);
PLAYER executeChoosenaAction(int gameOption, PLAYER player, CARD_STATISTIC cardsStatistics[], CARD* upperCard, bool* actionSuccess, int* specialCard, bool taki);
PLAYER takeCardExecution(PLAYER player, CARD_STATISTIC cardsStatistics[]);
PLAYER putCard(PLAYER player, CARD* upperCard, int choosenCard, bool* actionSuccess, int * specialCard);
PLAYER reArrangeCardDeck(PLAYER player, int choosenCard);
PLAYER takiCardActions(PLAYER player, CARD* upperCard, CARD_STATISTIC cardsStatistics[], int* specialCard);
PLAYER checkWin(PLAYER player, int specialCard, bool* win, CARD_STATISTIC cardsStatistics[], int amountOfplayers);
bool matchingColorValidation(char colorOne, char colorTwo);
CARD_STATISTIC* mergeTwoArrays(CARD_STATISTIC arrayOne[], int sizeOfArrayOne, CARD_STATISTIC arrayTwo[], int sizeOfArrayTwo);

void main()
{
	PLAYER* playersData;
	CARD upperCard;
	CARD_STATISTIC cardsStatistics[ALL_CARD_TYPES];
	int amountOfPlayers;
	srand(time(NULL));
	printStartingMessage();		// print the start message
	intializeStatisticsArray(cardsStatistics);	// intialize the statistics array
	playersData = preGamePreparations(&amountOfPlayers, cardsStatistics, &upperCard);	// prepare the game for playing
	playGame(playersData, amountOfPlayers, upperCard, cardsStatistics);	// play the game
	playersData = freeAllMemory(playersData, amountOfPlayers);	// free all the dynamic memory
	mergeSortForStatistics(cardsStatistics, ALL_CARD_TYPES);
	printCardStatistics(cardsStatistics, ALL_CARD_TYPES);
}

void printStartingMessage()		// a function to print start message
{
	printf("************  Welcome to TAKI game !!! ***********\n");

}

void checkDynamicAllocation(void* pointer)	//a function to check dynamic allocation(receives a pointer)
{
	if (pointer == NULL)
	{
		printf("dynamic allocation failed");
		exit(1);
	}
}

void intializeStatisticsArray(CARD_STATISTIC cardsStatistics[])	// a function to intialize StatisticsArray(receives the array and fills it up)
{
	int counter;
	for (counter = 0; counter < ALL_CARD_TYPES; counter++)
	{
		cardsStatistics[counter].cardNumber = counter + 1;	// the card numbers start from one unlike the indexes that start at 0
		cardsStatistics[counter].apearanceAmount = 0;
	}
}

void playGame(PLAYER* playersData, int amountOfPlayers, CARD upperCard, CARD_STATISTIC cardsStatistics[])	//the function used to play the game
{																											// receives the players data array, the amount of players in the game, the current upper card, and the statistics array 
	bool winnerExists = false;																				// the function ends when one of the players is the winner(has no cards and needs to take none)
	int currentPlayer = 1;	// the current player starts with 1
	int specialCard;		// the current active special card number
	char direction = FORWARD_DIRECTION;	// the advancing direction starts with a positive one
	while (winnerExists == false)		// while their is no winner continue playing
	{
		printf("\n");
		printf("Upper card:\n");
		printCard(upperCard);	// print the current upper card
		playersData = playTurn(playersData, currentPlayer, &upperCard,&specialCard, cardsStatistics);	// play the current players turn
		playersData[currentPlayer - 1] = checkWin(playersData[currentPlayer - 1], specialCard, &winnerExists, cardsStatistics, amountOfPlayers);	// check if the player won
		if (winnerExists == false)
		{
			currentPlayer = advanceTurn(specialCard, currentPlayer, amountOfPlayers, &direction);	// if their is no winner continue to the next player acording to the direction and active special card
			specialCard = 0;	
		}
	}
	printf("The winner is... %s! Congratulations!", playersData[currentPlayer - 1].playerName);	// print the winners name
}
void printCard(CARD cardToPrint)	// a function that prints the card it receives
{
	printf("*********\n");
	printf("*       *\n");
	if (cardToPrint.number <= REGULAR_CARDS)	// if the card received is a regular one(its number is below 10) print the card number in a certain way
	{
		printf("*   %d   *\n", cardToPrint.number);
	}
	else   //  if the card received isnt a regular one(its number is above 9) print the card number in a certain way using a function
	{
		printSpecialCardType(cardToPrint);
	}
	printf("*   %c   *\n", cardToPrint.color);	// print the cards color(the same way for all cards
	printf("*       *\n");
	printf("*********\n");

}
void printSpecialCardType(CARD cardToPrint)	// a function that prints the  special card name it receives(assists in the prnting of a card)
{
	switch (cardToPrint.number)
	{
	case(PLUS_CARD_NUMBER):
		printf("*   %s   *\n", PLUS_CARD);
		break;
	case(STOP_CARD_NUMBER):
		printf("*  %s *\n", STOP_CARD);
		break;
	case(CHANGE_DIRECTION_CARD_NUMBER):
		printf("*  %s  *\n", CHANGE_DIRECTION_CARD);
		break;
	case(TAKI_CARD_NUMBER):
		printf("*  %s *\n", TAKI_CARD);
		break;
	case(CHANGE_COLOR_CARD_NUMBER):
		printf("* %s *\n", CHANGE_COLOR_CARD);
		break;

	}

}

void printPlayerCards(PLAYER player)	// the function receives a player and prints his cards by order
{
	int cardCounter;
	for (cardCounter = 0; cardCounter < player.cardDeckLogicSize; cardCounter++)
	{
		printf("Card #%d:\n", cardCounter + 1);		//in order to print natural numbers(the indexes start from 0 and the numbers from 1)
		printCard(player.cardDeck[cardCounter]);	// print each card in the players deck
		printf("\n");
	}
}

void changeColorActions(CARD* upperCard)	// a function to print the menu displayed when selecting the color after the color change card was used
{											// the function receives a card pointer(to the change color card) and changes its color to the selected one then returns it by refference
	bool colorChanged = false;
	CARD localUpperCard = *upperCard;
	int choosenColor;
	printf("Please enter your color choice:\n");
	printColorOptions();		// print the available color options
	scanf(" %d", &choosenColor);
	while (colorChanged == false)	// while one of the colors wasnt selected continue to display the menu
	{
		switch (choosenColor)
		{
		case(YELLOW_COLOR_NUMBER):
			localUpperCard.color = YELLOW_COLOR;
			colorChanged = true;
			break;
		case(RED_COLOR_NUMBER):
			localUpperCard.color = RED_COLOR;
			colorChanged = true;
			break;
		case(BLUE_COLOR_NUMBER):
			localUpperCard.color = BLUE_COLOR;
			colorChanged = true;
			break;
		case(GREEN_COLOR_NUMBER):
			localUpperCard.color = GREEN_COLOR;
			colorChanged = true;
			break;
		default:
			printf("Invalid color! Try again.\n");
			printColorOptions();
			scanf(" %d", &choosenColor);
			break;
		}
	}
	*upperCard = localUpperCard;	//return the card with the new color by refference
}

void printColorOptions()	// a function print the available color options to pick
{
	printf("%d - Yellow\n", YELLOW_COLOR_NUMBER);
	printf("%d - Red\n", RED_COLOR_NUMBER);
	printf("%d - Blue\n", BLUE_COLOR_NUMBER);
	printf("%d - Green\n", GREEN_COLOR_NUMBER);
}

void changeDirectionCardActions(char* direction)	// a function that handles the direction change after the change direction card was used
{													// the function receives a direction(char) and changes it to the other available direction(returns the new direction by refference)
	switch (*direction)
	{
	case(FORWARD_DIRECTION):
		*direction = BACKWARDS_DIRECTION;
		break;
	case(BACKWARDS_DIRECTION):
		*direction = FORWARD_DIRECTION;
		break;
	default:
		break;
	}
}
void mergeSortForStatistics(CARD_STATISTIC cardsStatistics[], int size)	// a function that uses a merge sort to sort the statistics array ordered by the card appearance
{																		// the function receives the statistics array and its size and sorts it
	CARD_STATISTIC* temporaryArray = NULL;
	if (size <= 1)	// if the size is one or below no need to do anything
	{
		return;
	}
	else
	{
		mergeSortForStatistics(cardsStatistics, size / 2);	// sort one half
		mergeSortForStatistics(cardsStatistics + (size / 2), size - (size / 2));	// sort the other half
		temporaryArray = mergeTwoArrays(cardsStatistics, (size / 2), cardsStatistics + (size / 2), size - (size / 2));	// unite them in temporary array
		copyArray(cardsStatistics, size, temporaryArray);	// copy from the temporary array to the original one
		free(temporaryArray);	// free the temporary array after use
	}
}

void copyArray(CARD_STATISTIC cardsStatistics[],  int size, CARD_STATISTIC temporaryCardsStatistics[])	// a function that receives two CARD_STATISTIC arrays and their size and copies one into the other
{
	int index;
	for (index = 0; index < size; index++)
	{
		cardsStatistics[index] = temporaryCardsStatistics[index];
	}
}

void printCardStatistics(CARD_STATISTIC cardsStatistics[], int size)	// a function that prints the card statistics of the game
{																		// receives a sorted statistics array and its size and prints it
	int index;
	printf("\n");
	printf("************ Game Statistics ************\n");
	printf("Card # | Frequency\n");
	printf("__________________\n");
	for (index = 0; index < size; index++)
	{
		if (cardsStatistics[index].cardNumber <= REGULAR_CARDS)	// print the regular cards in a certain way
		{
			printf("   %d   |    %d\n", cardsStatistics[index].cardNumber, cardsStatistics[index].apearanceAmount);
		}
		else
		{
			switch (cardsStatistics[index].cardNumber)	// print each special card in its own way
			{
			case(PLUS_CARD_NUMBER):
				printf("   %s   |    %d\n", PLUS_CARD, cardsStatistics[index].apearanceAmount);
				break;
			case(STOP_CARD_NUMBER):
				printf(" %s  |    %d\n", STOP_CARD, cardsStatistics[index].apearanceAmount);
				break;
			case(CHANGE_DIRECTION_CARD_NUMBER):
				printf("  %s  |    %d\n", CHANGE_DIRECTION_CARD, cardsStatistics[index].apearanceAmount);
				break;
			case(TAKI_CARD_NUMBER):
				printf(" %s  |    %d\n", TAKI_CARD, cardsStatistics[index].apearanceAmount);
				break;
			case(CHANGE_COLOR_CARD_NUMBER):
				printf(" %s |    %d\n", CHANGE_COLOR_CARD, cardsStatistics[index].apearanceAmount);
				break;
			default:
				break;
			}
		}
	}
}

PLAYER* playTurn(PLAYER* playersData, int currentPlayer, CARD* upperCard, int* specialCard, CARD_STATISTIC cardsStatistics[])	//  a function the handles the current players turn
{																																// receives the players array, the current players number, the upper card by refference, the current active special card by refference, and the statistics array
	bool actionSuccess;		// a variable to determine action success (actions like picking a card)																									// returns the players data array(changing the current players game data)
	int gameOption, localSpecialCard = 0;																						// returns by refference the new upper card and the new active special card
	printf("%s's turn:\n\n", playersData[currentPlayer - 1].playerName);	// print the current player name
	printPlayerCards(playersData[currentPlayer - 1]);	// print the cards of the current player
	gameOption = gameOptionsMenuDisplay(playersData[currentPlayer - 1], localSpecialCard);	// print the options available to the player(pick a card or put a card) and validate the selected one
	playersData[currentPlayer - 1] = executeChoosenaAction(gameOption, playersData[currentPlayer - 1], cardsStatistics, upperCard, &actionSuccess, &localSpecialCard, false);	// execute the selected option(pick a card or put a card)
	while (actionSuccess == false)	// while the action wasnt succesful(like the card the player wanted to put didnt match the color of the upper one) continue to request an action
	{
		gameOption = gameOptionsMenuDisplay(playersData[currentPlayer - 1], localSpecialCard);
		playersData[currentPlayer - 1] = executeChoosenaAction(gameOption, playersData[currentPlayer - 1], cardsStatistics, upperCard, &actionSuccess, &localSpecialCard, false);
	}
	if (playersData[currentPlayer - 1].cardDeckLogicSize > 0)	// if the deck of the current player isnt empty(possible win), handle the taki and change color cards if they were used
	{
		if (localSpecialCard == TAKI_CARD_NUMBER)	// if taki card was activated handle it
		{
			playersData[currentPlayer - 1] = takiCardActions(playersData[currentPlayer - 1], upperCard, cardsStatistics, &localSpecialCard);
		}
		if (localSpecialCard == CHANGE_COLOR_CARD_NUMBER)	// if change color card was activated handle it
		{
			changeColorActions(upperCard);
		}
	}
	*specialCard = localSpecialCard;
	return(playersData);
}

int gameOptionsMenuDisplay(PLAYER player, int specialCard)		// a function to display and validate the available game options acording to the current special card
{																// receives the player and the current special card and returns a valid game option(int)
	int gameOption;
	if (specialCard == TAKI_CARD_NUMBER)	// if taki is active print a different message
	{
		printf("Please enter %d if you want to finish your turn\n", FINISH_TURN_ACTION);
	}
	else
	{
		printf("Please enter %d if you want to take a card from the deck\n", TAKE_CARD_ACTION);;
	}
	printf("or 1-%d if you want to put one of your cards in the middle:\n", player.cardDeckLogicSize);
	scanf(" %d", &gameOption);
	gameOptionValidation(player, &gameOption, specialCard);	// validate the selected game option
	return (gameOption);

}

void gameOptionValidation(PLAYER player, int* gameOption, int specialCard)	// a function to validate the available game options
{																			// receives a game option, the player, and the special card and continues to request a game option until it is correct 
	int localGameOption = *gameOption;										// returns a valid option by refference
	while (localGameOption < 0 || localGameOption > player.cardDeckLogicSize)
	{
		printf("Invalid choice! Try again.\n");
		if (specialCard == TAKI_CARD_NUMBER)
		{
			printf("Please enter %d if you want to finish your turn\n", FINISH_TURN_ACTION);
		}
		else
		{
			printf("Please enter %d if you want to take a card from the deck\n", TAKE_CARD_ACTION);
		}
		printf("or 1-%d if you want to put one of your cards in the middle:\n", player.cardDeckLogicSize);
		scanf(" %d", &localGameOption);
	}
	*gameOption = localGameOption;
}

int recieveAndCheckPlayerAmount()	// a function that requests a player amount than returns it when it is valid(above zero)
{
	int playerAmount;
	printf("Please enter the number of players:\n");
	scanf(" %d", &playerAmount);
	while (playerAmount <= 0)
	{
		printf("incorrect Number! please enter a positive player number\n");
		scanf(" %d", &playerAmount);
	}
	return (playerAmount);
}
int advanceTurn(int specialCard, int currentPlayer, int amountOfPlayers, char* direction)	// a function that advances the current turn acording to direction amount of players and the active special card
{																							// receives all of them but the direction by value than returns by value the number of the next player and the new direction by refference
	int nextPlayerNumber = 0;
	if (specialCard != 0)	// if a special card was activated use it in the calculation of the next player number
	{
		switch (specialCard)
		{
		case(PLUS_CARD_NUMBER):
			nextPlayerNumber = currentPlayer;
			break;
		case(STOP_CARD_NUMBER):
			nextPlayerNumber = stopCardActions(currentPlayer, amountOfPlayers, *direction);
			break;
		case(CHANGE_DIRECTION_CARD_NUMBER):
			changeDirectionCardActions(direction);
			break;
		default:
			break;
		}
	}
	if(nextPlayerNumber == 0)	//next player wasnt choosen before(no special card was used this turn)
	{
		nextPlayerNumber = calculateNextPlayerNumber(currentPlayer, amountOfPlayers, *direction, REGULAR_ADVANCEMENT);
	}

	return (nextPlayerNumber);
}

int stopCardActions(int currentPlayer, int amountOfPlayers, char direction )	// a function that handles the stop card when it is used (recieves the current player number the total amount of them and the direction
{																				// returns the next players number
	int nextPlayerNumber;
	nextPlayerNumber = calculateNextPlayerNumber(currentPlayer, amountOfPlayers, direction, STOP_CARD_ADVANCEMENT);
	return (nextPlayerNumber);
}

int calculateNextPlayerNumber(int currentPlayer, int amountOfPlayers, char direction, int advancement)	// a function the calculates and returns the next players number
{																										// receives the current players number, the total amount of them, the current direction, and the advancement(how much players to move starting from the current one)
	int nextPlayerNumber;																				// returns the next players number
	if (amountOfPlayers <= advancement)		// if the amount of players to move is bigger than the total amount of players, no need to move to next player
	{
		nextPlayerNumber = currentPlayer;
	}
	else
	{
		if (direction == FORWARD_DIRECTION)
		{
			if ((currentPlayer + advancement) > amountOfPlayers)	// if reached the highest player number
			{
				nextPlayerNumber = (currentPlayer + advancement) % amountOfPlayers;
			}
			else
			{
				nextPlayerNumber = currentPlayer + advancement;
			}
		}
		else  // backwards direction
		{
			if ((currentPlayer - advancement) < 1)		// if reached the lowerst player number
			{
				nextPlayerNumber = (currentPlayer - advancement) + amountOfPlayers;
			}
			else
			{
				nextPlayerNumber = currentPlayer - advancement;
			}
		}
	}
	return (nextPlayerNumber);
}

PLAYER* preGamePreparations(int* amountOfPlayers, CARD_STATISTIC cardsStatistics[], CARD* upperCard)	// a function that handles the pre game data collection and intialization of the players array and the upper card
{																										// receives and returns the amount of players and the upper card by refference and also updates the statistics array accordingly
	PLAYER* playersData;
	int playerAmount = recieveAndCheckPlayerAmount();	// request and validate the players amount
	playersData = receivePlayersData(playerAmount);	// intialize players data array and receive each players name
	playersData = intializeGame(playerAmount, playersData, upperCard, cardsStatistics);	//give all players their cards and generate an upper card
	*amountOfPlayers = playerAmount;

	return(playersData);
}

PLAYER* receivePlayersData(int players) // a function that creates a dynamic array and requests each player name and fills the array wth it 
{										// receives the amount of players and returns the created players dynamic array
	PLAYER* playersData;
	int playerCounter;
	playersData = (PLAYER*)malloc(players * sizeof(PLAYER));
	checkDynamicAllocation(playersData);

	for (playerCounter = 0; playerCounter < players; playerCounter++)
	{
		printf("Please enter the first name of player #%d:\n", playerCounter + 1);	// + 1 in order to display player numbers in natural numbers
		scanf(" %s", playersData[playerCounter].playerName);
	}

	return (playersData);
}

PLAYER* intializeGame(int players, PLAYER* playersData, CARD* upperCard, CARD_STATISTIC cardsStatistics[])	// a function that handles all the cards creation (the ones the players have and the upper card)
{																											// receives a statistics array, the amount of players, and returns the upper card by refference and the players data array by value  
	*upperCard = generateCard(REGULAR_CARDS, cardsStatistics);	// generate an upper card
	int playerCounter;
	for (playerCounter = 0; playerCounter < players; playerCounter++)
	{
		playersData[playerCounter] = generateInitialCards(playersData[playerCounter], cardsStatistics);	//give each player their cards
	}
	return(playersData);
}

PLAYER* freeAllMemory(PLAYER* playersData, int amountOfPlayers)	// a function that handles the memory freeing of the players data array and each players card deck
{																// receives the players data pointer and the amount of players then returns an empty players data array
	int playerCounter;
	for (playerCounter = 0; playerCounter < amountOfPlayers; playerCounter++)
	{
		free(playersData[playerCounter].cardDeck);	// free each players card deck
	}
	free(playersData);
	return (playersData);
}

CARD generateCard(int cardNumberToGenerateUpTo, CARD_STATISTIC cardsStatistics[])	// a function that handles a single card generation and statistics update 
{																					//receives a number to generate card numbers up to and returns the generated card
	int randomNumber;
	CARD generatedCard;
	randomNumber = (rand() % cardNumberToGenerateUpTo) + 1;	// in order to generate natural numbers 
	generatedCard.number = randomNumber;
	cardsStatistics[randomNumber - 1].apearanceAmount++;	// update generated card appearance amount
	if (generatedCard.number == CHANGE_COLOR_CARD_NUMBER)
	{
		generatedCard.color = NO_COLOR;
	}
	else
	{
		randomNumber = (rand() % AMOUNT_OF_COLORS) + 1;	// in order to generate natural numbers 
		generatedCard.color = numberToCharConversion(randomNumber);
	}
	return (generatedCard);
}

char numberToCharConversion(int number)	// a function that convertes the color number to char that is stored and displayed
{										// receives a color number and returns the matching char
	char charTheNumberRepresents = ' ';	//intialized to avoid using uninitlized memory warning
	switch (number)
	{
	case(BLUE_COLOR_NUMBER):
		charTheNumberRepresents = BLUE_COLOR;
		break;
	case(RED_COLOR_NUMBER):
		charTheNumberRepresents = RED_COLOR;
		break;
	case(GREEN_COLOR_NUMBER):
		charTheNumberRepresents = GREEN_COLOR;
		break;
	case(YELLOW_COLOR_NUMBER):
		charTheNumberRepresents = YELLOW_COLOR;
		break;
	default:
		break;
	}
	return (charTheNumberRepresents);
}

PLAYER generateInitialCards(PLAYER player, CARD_STATISTIC cardsStatistics[])	// a function that generates a single players cards
{																				// receives and returns the player by value
	int cardCounter;
	player.cardDeck = (CARD*)malloc(INITIAL_CARDS_AMOUNT * sizeof(CARD));
	checkDynamicAllocation(player.cardDeck);
	player.cardDeckLogicSize = INITIAL_CARDS_AMOUNT;
	player.cardDeckSize = INITIAL_CARDS_AMOUNT;
	for (cardCounter = 0; cardCounter < INITIAL_CARDS_AMOUNT; cardCounter++)	// generate cards acording to intial cards amount
	{
		player.cardDeck[cardCounter] = generateCard(ALL_CARD_TYPES, cardsStatistics);
	}

	return (player);
}

PLAYER myRealloc(PLAYER player)	// a function the expands the players card deck when needed
{								// receives and returns the player by value
	int cardCounter;
	CARD* newCardDeck;
	newCardDeck = (CARD*)malloc((player.cardDeckSize * 2) * sizeof(CARD));
	checkDynamicAllocation(newCardDeck);
	for (cardCounter = 0; cardCounter < player.cardDeckLogicSize; cardCounter++)
	{
		newCardDeck[cardCounter] = player.cardDeck[cardCounter];
	}
	free(player.cardDeck);
	player.cardDeck = newCardDeck;
	player.cardDeckSize = (player.cardDeckSize * 2);

	return(player);
}

PLAYER executeChoosenaAction(int gameOption, PLAYER player, CARD_STATISTIC cardsStatistics[], CARD* upperCard, bool* actionSuccess, int * specialCard, bool taki)	// a function that handles the execution of the received game option 
{																																									

	if (gameOption == TAKE_CARD_ACTION && taki == false)	// cannot take cards while taki is active
	{
		player = takeCardExecution(player, cardsStatistics);
		*actionSuccess = true;	// action was a success
	}
	else
	{
		player = putCard(player, upperCard, gameOption, actionSuccess, specialCard);	// handle the put card proccess
	}
	return(player);
}

PLAYER takeCardExecution(PLAYER player, CARD_STATISTIC cardsStatistics[])	// a function that handles the proccess of receiving a new card from the deck
{																			// returns the player by value
	CARD newCard = generateCard(ALL_CARD_TYPES, cardsStatistics);
	if (player.cardDeckLogicSize == player.cardDeckSize)
	{
		player = myRealloc(player);
	}
	player.cardDeck[player.cardDeckLogicSize] = newCard;
	player.cardDeckLogicSize++;
	return (player);
}

PLAYER putCard(PLAYER player, CARD* upperCard, int choosenCard, bool* actionSuccess, int* specialCard)	// a function that handles the proccess of puting a card 
{																										// returns the player by value
	CARD localUpperCard = *upperCard;
	CARD theChoosenCard = player.cardDeck[choosenCard - 1];

	if (theChoosenCard.number <= REGULAR_CARDS)	
	{
		if (matchingColorValidation(theChoosenCard.color, localUpperCard.color) == true)	// if the color of the upper card matches the selected card
		{
			localUpperCard = theChoosenCard;
			player = reArrangeCardDeck(player, choosenCard);	// rearrange card deck after putting the card
			*actionSuccess = true;
		}
		else
		{
			*actionSuccess = false;
			printf("Invalid card! Try again.\n");
		}
	}
	else
	{
		if (matchingColorValidation(theChoosenCard.color, localUpperCard.color) == true)
		{
			localUpperCard = theChoosenCard;
			player = reArrangeCardDeck(player, choosenCard);
			*specialCard = theChoosenCard.number;
			*actionSuccess = true;
		}
		else
		{
			*actionSuccess = false;
			printf("Invalid card! Try again.\n");
		}
	}

	*upperCard = localUpperCard;
	return(player);
}

PLAYER reArrangeCardDeck(PLAYER player, int choosenCard)	// a function that rearranges the deck after having minus one card
{															// returns the player by value
	while (choosenCard < player.cardDeckLogicSize)
	{
		player.cardDeck[choosenCard - 1] = player.cardDeck[choosenCard];
		choosenCard++;
	}
	player.cardDeckLogicSize--;

	return(player);
}
PLAYER takiCardActions(PLAYER player, CARD* upperCard, CARD_STATISTIC cardsStatistics[], int* specialCard)	// a function that handles the taki card when it is used
{																											// returns the player by value
	bool actionSuccess;
	int gameOption;
	printf("\n");
	printf("Upper card:\n");
	printCard(*upperCard);
	printf("%s's turn:\n\n", player.playerName);
	printPlayerCards(player);
	gameOption = gameOptionsMenuDisplay(player, TAKI_CARD_NUMBER);
	while (gameOption != FINISH_TURN_ACTION && player.cardDeckLogicSize > 0 && (*upperCard).number != CHANGE_COLOR_CARD_NUMBER)	// while the player havent finished the turn and isnt out of cards and hasnt put on the change color card continue to request cards to put on the deck
	{
		player = executeChoosenaAction(gameOption, player, cardsStatistics, upperCard, &actionSuccess, specialCard, true);
		if (actionSuccess == true && player.cardDeckLogicSize > 0)
		{
			printf("\n");
			printf("Upper card:\n");
			printCard(*upperCard);
			printf("%s's turn:\n\n", player.playerName);
			printPlayerCards(player);
		}
		if (player.cardDeckLogicSize > 0 && (*upperCard).number != CHANGE_COLOR_CARD_NUMBER)
		{
			gameOption = gameOptionsMenuDisplay(player, TAKI_CARD_NUMBER);
		}
	}
	return (player);
}
PLAYER checkWin(PLAYER player, int specialCard, bool* win, CARD_STATISTIC cardsStatistics[], int amountOfplayers)	// a function that checks if the current player has won
{																													// returns the winning bool by reffernce and the player by value
	if (player.cardDeckLogicSize == 0)
	{
		if ((specialCard == STOP_CARD_NUMBER && amountOfplayers == 2) || specialCard == PLUS_CARD_NUMBER)	// if stop card or plus card was put last give the player an extra card
		{
			player.cardDeck[0] = generateCard(ALL_CARD_TYPES, cardsStatistics);
			player.cardDeckLogicSize++;
		}
		else
		{
			*win = true;
		}
	}
	return (player);
}

bool matchingColorValidation(char colorOne, char colorTwo)	// a fucntion that checks the maching of the color of cards
{															// returns true if matches
	bool match = false;
	if (colorOne == colorTwo || colorOne== NO_COLOR)	// if the colors match or one of them is without color(change color card) return true
	{
		match = true;
	}
	return (match);
}

CARD_STATISTIC* mergeTwoArrays(CARD_STATISTIC arrayOne[], int sizeOfArrayOne, CARD_STATISTIC arrayTwo[], int sizeOfArrayTwo)	// a function that sorts two CARD_STATISTIC  arrays using merge sort
{
	CARD_STATISTIC* temporaryStatArray = (CARD_STATISTIC*)malloc((sizeOfArrayOne + sizeOfArrayTwo) * sizeof(CARD_STATISTIC));
	int arrayOneIndex = 0, arrayTwoIndex = 0, temporaryArrayIndex = 0;
	checkDynamicAllocation(temporaryStatArray);
	while (arrayOneIndex < sizeOfArrayOne && arrayTwoIndex < sizeOfArrayTwo) // merge start
	{
		if (arrayOne[arrayOneIndex].apearanceAmount >= arrayTwo[arrayTwoIndex].apearanceAmount)
		{
			temporaryStatArray[temporaryArrayIndex] = arrayOne[arrayOneIndex];
			arrayOneIndex++;
			temporaryArrayIndex++;
		}
		else
		{
			if (arrayOne[arrayOneIndex].apearanceAmount < arrayTwo[arrayTwoIndex].apearanceAmount)
			{
				temporaryStatArray[temporaryArrayIndex] = arrayTwo[arrayTwoIndex];
				arrayTwoIndex++;
				temporaryArrayIndex++;
			}
		}
	}

	while (arrayOneIndex < sizeOfArrayOne )
	{
		temporaryStatArray[temporaryArrayIndex] = arrayOne[arrayOneIndex];
		arrayOneIndex++;
		temporaryArrayIndex++;
	}

	while (arrayTwoIndex < sizeOfArrayTwo)
	{
		temporaryStatArray[temporaryArrayIndex] = arrayTwo[arrayTwoIndex];
		arrayTwoIndex++;
		temporaryArrayIndex++;
	}

	return (temporaryStatArray);

}