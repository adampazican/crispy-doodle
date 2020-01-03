
#include <iostream>
#include <time.h>
#include "../common.h"

#define FIGURES_FOR_PLAYERS 4
#define NUMBER_OF_PLAYERS 4
#define HOUSE_SIZE 4
#define GAME_SIZE 32

#define size(array) (int) (sizeof(array) / sizeof(array[0]))

using namespace std;

struct Square {
	char character;
	Color color;
};

struct Coordinates
{
    int x,y;
};

Coordinates getSquareInFinish(int possition, int playerId)
{
	int x = 0;
	int y = 0;
    
	if (playerId == 0)
	{
		x = 5;
		y = possition;
	}
	else if (playerId == 1)
	{
		y = 5;
		x = 10 - possition;
	}
	else if (playerId == 2)
	{
		x = 5;
		y = 10 - possition;
		
	}
	else 
	{
		y = 5;
		x = possition;
	}
    
    Coordinates coordinates = {x,y};
	return coordinates;
}

Coordinates getSquareInHouse(int possition, int playerId)
{
	int x = 0;
	int y = 0;
    
	switch (possition)
	{
        case 1:
		x = 0;
		y = 0;
		break;
        case 2:
		x = 1;
		y = 0;
		break;
        case 3:
		x = 0;
		y = 1;
		break;
        case 4:
		x = 1;
		y = 1;
		break;
	}
	
	switch (playerId)
	{
        case 0:
		break;
        case 1:
		x += 9;
		break;
        case 2:
		x += 9;
		y += 9;
		break;
        case 3:
		y += 9;
		break;
	}
    
    Coordinates coordinates = {x,y};
	return coordinates;
}

Coordinates getSquareInGame(int possition,int playerId) {
	int numbers[] = { 2, 4, 4, 2, -4, 4, -2, -4, -4, -2, 4, -4};
    
	int pom = possition;
	int pomm = possition;
    
	while (pomm > 0)
	{
		pomm -= 10;
		if (pomm >= 0)
		{
			pom++;
			pomm++;
		}
        
		
	}
    
	pom += playerId * 10 ;
    
	int x = 4;
	int y = 0;
	bool change = true;
	while (pom != 0) 
	{
		for (int i = 0; i < size(numbers); i++)
		{
			int actualNumber = numbers[i];
            
			while (actualNumber != 0)
			{
				if (change)
				{
					x += numbers[i] > 0 ? 1 : -1;
				}
				else
				{
					y += numbers[i] > 0 ? 1 : -1;
				}
				actualNumber = actualNumber > 0 ? actualNumber - 1 : actualNumber + 1;
				pom--;
				if (pom == 0)
				{
                    Coordinates coordinates = {x,y};
                    return coordinates;
				}
			}
			change = change ? false : true;
		}
        
	}
    
    Coordinates coordinates = {x,y};
	return coordinates;
}



void vykresli(Square squeres[11][11]) 
{
	for (int i = 0; i < 11; i++)
	{
		if (i == 0)
		{
			for (int k = 0; k < 13; k++)
			{
				cout << " #";
			}
			cout << endl;
		}
		cout << " #";
        
		for (int j = 0; j < 11; j++)
		{
			/*			foreground background
    black        30         40
    red          31         41
    green        32         42
    yellow       33         43
    blue         34         44
    magenta      35         45
    cyan         36         46
    white        37         47
    */
			cout << ' ';
			switch (squeres[i][j].color)
			{
                case Color::BLUE:
				cout << "\033[1;34m"<< squeres[i][j].character  << "\033[0m";
				break;
                case Color::GREEN:
				cout << "\033[1;32m" << squeres[i][j].character << "\033[0m";
				break;
                case Color::YELLOW:
				cout << "\033[1;33m" << squeres[i][j].character << "\033[0m";
				break;
                case Color::RED:
				cout << "\033[1;31m" << squeres[i][j].character << "\033[0m";
				break;
                case Color::WHITE:
				cout << squeres[i][j].character;
				break;
			}
		}
		cout << " #";
		cout << endl;
        
		if (i == 10)
		{
			for (int k = 0; k < 13; k++)
			{
				cout << " #";
			}
			cout << endl;
		}
	}
}

int getNumber()
{
	return rand() % 6 + 1;
}

bool allInHouse(Player player)
{
	int number = 0;
    
	for (int i = 0; i < HOUSE_SIZE; i++)
	{
		if (player.figurines[i].figurineState == FigureState::IN_HOUSE)
			number++;
	}
    
	return number == 4 ? true : false;
}

bool playable(Player player, int numFig, int roll) {
    
	if (player.figurines[numFig].figurineState == FigureState::IN_HOUSE && roll != 6)
		return false;
	else if (player.figurines[numFig].figurineState == FigureState::IN_FINISH)
	{
		if (player.figurines[numFig].position + roll > 4)
			return false;
		else
		{
			struct Coordinates poss = getSquareInFinish(player.figurines[numFig].position + roll, player.playerId);
            
			{
				for (int i = 0; i < HOUSE_SIZE; i++)
				{
					if (player.figurines[i].figurineState == FigureState::IN_FINISH && i != numFig)
					{
						struct Coordinates poss2 = getSquareInFinish(player.figurines[i].position, player.playerId);
                        
						if (poss.x == poss2.x && poss.y == poss2.y)
							return false;
					}
				}
			}
		}
	}
	else if (player.figurines[numFig].figurineState == FigureState::IN_GAME)
	{
		struct Coordinates poss = getSquareInGame(player.figurines[numFig].position + roll, player.playerId);
        
		//pytam sa ci ide uz do ciela, v cieli sa uz neda posunut
		if (player.figurines[numFig].position + roll <= 37)
		{
			//overujem ci uz nie je na policku figurka rovnakej farnby
			for (int i = 0; i < HOUSE_SIZE; i++)
			{
                
				if (player.figurines[i].figurineState == FigureState::IN_GAME && i != numFig) {
					struct Coordinates poss2 = getSquareInGame(player.figurines[i].position, player.playerId);
                    
					if (poss.x == poss2.x && poss.y == poss2.y)
						return false;
				}
			}
		}
		else {
			//ci mu nezavadza figurka v cieli a moze ist do ciela
			int possFinish = player.figurines[numFig].position - 37 + roll;
			if (possFinish > 4)
				return false;
			else 
			{
				struct Coordinates poss = getSquareInFinish(possFinish, player.playerId);
                
				for (int  i = 0; i < HOUSE_SIZE; i++)
				{
					if (player.figurines[i].figurineState == FigureState::IN_FINISH && i != numFig)
					{
						struct Coordinates poss2 = getSquareInFinish(player.figurines[i].position, player.playerId);
                        
						if (poss.x == poss2.x && poss.y == poss2.y)
							return false;
					}
				}
			}
		}
	}
    
	return true;
}

int numberOfplayable(Player player, int roll)
{
	int number = 4;
	for (int i = 0; i < HOUSE_SIZE; i++)
	{
		if (!playable(player, i, roll))
			number--;
	}
    
	return number;
}

bool allInFinish(Player player)
{
	int number = 0;
    
	for (int i = 0; i < HOUSE_SIZE; i++)
	{
		if (player.figurines[i].figurineState == FigureState::IN_FINISH)
			number++;
	}
    
	return number == 4 ? true : false;
}

void PressEnterToContinue()
{
	int c;
	printf("Press ENTER to roll. ");
	fflush(stdout);
	do c = getchar(); while ((c != '\n') && (c != EOF));
}

void PressEnterToEnd()
{
	int c;
	printf("Press ENTER to finsih turn.");
	fflush(stdout);
	do c = getchar(); while ((c != '\n') && (c != EOF));
}

Color getColorByTurnId(int id)
{
	if (id == 0)
		return Color::RED;
	else if (id == 1)
		return Color::GREEN;
	else if (id == 2)
		return Color::YELLOW;
	else
		return Color::BLUE;
}

int main()
{
	srand(time(NULL));
    
	Game game;
	game.gameState = GameState::PLAYING;
	game.turnId = 0;
    
	Square squeres[11][11];
    
	for (int  i = 0; i < 11; i++)
	{
		for (int  j = 0; j < 11; j++)
		{
			
			if (i > 3 && i < 7)				//Hracie policka
			{
				if (i == 5 && j == 5)
				{
					squeres[i][j].character = ' ';
					squeres[i][j].color = Color::WHITE;
				}
				else
				{
					squeres[i][j].character = 'O';
					squeres[i][j].color = Color::WHITE;
				}
			}
			else if (j > 3 && j < 7)				//Hracie policka
			{
				squeres[i][j].character = 'O';
				squeres[i][j].color = Color::WHITE;
			}
			else if (i < 2 && j < 2)				//IN_HOUSE_POLICKA
			{
				squeres[i][j].character = 'O';
				squeres[i][j].color = Color::RED;
			}
			else if (i > 8 && j < 2)
			{
				squeres[i][j].character = 'O';
				squeres[i][j].color = Color::BLUE;
			}
			else if (i < 2 && j >  8)
			{
				squeres[i][j].character = 'O';
				squeres[i][j].color = Color::GREEN;
			}
			else if (i > 8 && j > 8)
			{
				squeres[i][j].character = 'O';
				squeres[i][j].color = Color::YELLOW;
			}
			else			// prazdne policka
			{
				squeres[i][j].character = ' ';
				squeres[i][j].color = Color::WHITE;
			}
            
			if (i == 5 && j > 0 && j < 5)				//INFINISH policka
				squeres[i][j].color = Color::BLUE;
			if (i == 5 && j > 5 && j < 10)
				squeres[i][j].color = Color::GREEN;
			if (j == 5 && i > 0 && i < 5)
				squeres[i][j].color = Color::RED;
			if (j == 5 && i > 5 && i < 10)
				squeres[i][j].color = Color::YELLOW;
            
			if (j == 4 && i == 0)
			{
				squeres[i][j].color = Color::RED;
				squeres[i][j].character = '0';
			}
			if (j == 0 && i == 6)
			{
				squeres[i][j].color = Color::BLUE;
				squeres[i][j].character = '0';
			}
			if (j == 10 && i == 4)
			{
				squeres[i][j].color = Color::GREEN;
				squeres[i][j].character = '0';
			}
			if (j == 6 && i == 10)
			{
				squeres[i][j].color = Color::YELLOW;
				squeres[i][j].character = '0';
			}
		}
	}
    
	for (int i = 0; i <  FIGURES_FOR_PLAYERS ; i++)
	{
		game.players[i].playerId = i;
        
		for (int j = 0; j < FIGURES_FOR_PLAYERS; j++)
		{
			game.players[i].figurines[j].figurineState = FigureState::IN_HOUSE;
			game.players[i].figurines[j].position = j + 1;
			
			Coordinates suradnice = getSquareInHouse(j + 1, i);
			squeres[suradnice.y][suradnice.x].character = '0' + game.players[i].figurines[j].position;
		}
	}
    
	game.turnId = 0;
    
	int umiestnenie[4];
	bool ukoncene[4];
	int index = 0;
    
	for (int  i = 0; i < NUMBER_OF_PLAYERS; i++)
	{
		umiestnenie[i] = -1;
		ukoncene[i] = false;
	}
    
	while (game.gameState != GameState::FINISHED)
	{
		system("clear");
		vykresli(squeres);
        
		game.turnId %= 4;
        
		if (game.turnId == 0)
			cout << "\033[1;31m" << "Red on the turn" << "\033[0m" << endl;
		else if (game.turnId == 1)
			cout << "\033[1;32m" << "Green on the turn." << "\033[0m" << endl;
		else if (game.turnId == 2)
			cout << "\033[1;33m" << "Yellow on the turn." << "\033[0m" << endl;
		else if (game.turnId == 3)
			cout << "\033[1;34m" << "Blue on the turn." << "\033[0m" << endl;
        
		//ak ma vsetkych v domceku caka kym nehodi 6, hadze 3 krat
		int number = 0;
		int numberOfSix = 0;
        
		//ak v tych troch pokusoch padne 3 skonci
		if (allInHouse(game.players[game.turnId]))
		{
			for (int i = 0; i < 3; i++)
			{
				PressEnterToContinue();
				number = getNumber();
				cout << "You got number " << number << endl;
				
				if (number == 6)
				{
					numberOfSix++;
					break;
				}
                
			}
		}
		else if (allInFinish(game.players[game.turnId]))
		{
			numberOfSix = -1;
			number = 0;
		}
		else {		//ak nema vsetkych v domceku hadze iba raz
			PressEnterToContinue();
			number = getNumber();
			cout << "You got number " << number << endl;
			if (number == 6)
				numberOfSix++;
		}
		//ak hodi sestku hadze znova
		while (number == 6)
		{
			PressEnterToContinue();
			number = getNumber();
			cout << "You got number " << number << endl;
			if (number == 6)
				numberOfSix++;
		}
        
		while (numberOfSix >= 0)
		{
			int maxNumber = 0;
            
			if (numberOfSix > 0)
				maxNumber = 6;
			else
				maxNumber = number;
            
			if (allInHouse(game.players[game.turnId]) && maxNumber != 6)
			{
				PressEnterToEnd();
			}
			else if (allInFinish(game.players[game.turnId]))
			{
				PressEnterToEnd();
			}
			else if ( numberOfplayable(game.players[game.turnId],maxNumber) == 0 )
			{
				PressEnterToEnd();
			}
			else 
			{
				bool iSplayable = false;
				int figNumber = 0;
                
				while (!iSplayable)
				{
					cout << "Chose which figurines move 1,2,3 or 4 about " << maxNumber << endl;
					cin >> figNumber;
					figNumber--;
					if (figNumber < 4 && playable(game.players[game.turnId], figNumber, maxNumber))
						iSplayable = true;
					else
						cout << "you cant play with figure number " << figNumber + 1 << endl;
				}
				//ak je v domceku a hodim 6 tak ho pripravim na start
				if (game.players[game.turnId].figurines[figNumber].figurineState == FigureState::IN_HOUSE && maxNumber == 6)
				{
					Coordinates poss = getSquareInHouse(game.players[game.turnId].figurines[figNumber].position, game.turnId);
					squeres[poss.y][poss.x].character = '0';
                    
					game.players[game.turnId].figurines[figNumber].position = 0;
					game.players[game.turnId].figurines[figNumber].figurineState = FigureState::IN_GAME;
                    
					poss = getSquareInGame(game.players[game.turnId].figurines[figNumber].position, game.turnId);
					squeres[poss.y][poss.x].character = '0' + (figNumber + 1);
                    
					system("clear");
					vykresli(squeres);
                    
				}
				else if (game.players[game.turnId].figurines[figNumber].figurineState == FigureState::IN_GAME)
				{
					//vyprazdni policko
					Coordinates poss = getSquareInGame(game.players[game.turnId].figurines[figNumber].position, game.turnId);
                    
					squeres[poss.y][poss.x].character = 'O';
					squeres[poss.y][poss.x].color = Color::WHITE;
                    
					//ak je to pociatocne policko nemeni farbu
					if (game.players[game.turnId].figurines[figNumber].position == 0)
					{
						int figInStart = 0;
						for (int i = 0; i < HOUSE_SIZE; i++)
						{
							if (game.players[game.turnId].figurines[i].position == 0 && figNumber != i) {
								figInStart = i + 1;
							}
						}
                        
						squeres[poss.y][poss.x].character = '0' + figInStart;
						squeres[poss.y][poss.x].color = getColorByTurnId(game.turnId);
                        
					}
                    
					//overenie ci nejdem už do domceka
					if (game.players[game.turnId].figurines[figNumber].position + maxNumber > 37)
					{
						//presunie do ciela
						game.players[game.turnId].figurines[figNumber].figurineState = FigureState::IN_FINISH;
						game.players[game.turnId].figurines[figNumber].position += maxNumber - 37;
						poss = getSquareInFinish(game.players[game.turnId].figurines[figNumber].position, game.turnId);
						squeres[poss.y][poss.x].character = '0' + figNumber + 1;
					}
					else
					{
						//posunie na nove miesto na hracej ploche
						poss = getSquareInGame(game.players[game.turnId].figurines[figNumber].position + maxNumber, game.turnId);
						game.players[game.turnId].figurines[figNumber].position += maxNumber;
						squeres[poss.y][poss.x].character = '0' + figNumber + 1;
						squeres[poss.y][poss.x].color = getColorByTurnId(game.turnId);
					}
					
                    
					//overenie ci som nevykopol figurku
                    
					for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
					{
						for (int j = 0; j < HOUSE_SIZE; j++)
						{
							if (game.turnId != i && game.players[i].figurines[j].figurineState == FigureState::IN_GAME)
							{
								Coordinates pom1 = getSquareInGame(game.players[i].figurines[j].position, i);
                                
								if (poss.x == pom1.x && poss.y == pom1.y)
								{
									cout << "vykopol si figurku" << endl;
									game.players[i].figurines[j].position = j + 1;
									game.players[i].figurines[j].figurineState = FigureState::IN_HOUSE;
									pom1 = getSquareInHouse(game.players[i].figurines[j].position, i);
									squeres[pom1.y][pom1.x].character = '0' + (j + 1);
								}
							}
						}
					}
                    
					system("clear");
					vykresli(squeres);
					PressEnterToEnd();
                    
				}
				else if (game.players[game.turnId].figurines[figNumber].figurineState == FigureState::IN_FINISH)
				{
					Coordinates poss = getSquareInFinish(game.players[game.turnId].figurines[figNumber].position, game.turnId);
					squeres[poss.y][poss.x].character = 'O';
                    
					poss = getSquareInFinish(game.players[game.turnId].figurines[figNumber].position + maxNumber, game.turnId);
					squeres[poss.y][poss.x].character = '0' + figNumber + 1;
                    
					game.players[game.turnId].figurines[figNumber].position += maxNumber;
				}
                
			}
            
			numberOfSix--;
		}
        
		int playerFinished = 0;
        
		for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
		{
			if (allInFinish(game.players[i]))
				playerFinished++;
		}
        
		if (playerFinished == 4)
			game.gameState = GameState::FINISHED;
        
		if (allInFinish(game.players[game.turnId]))
		{
			if (!ukoncene[game.turnId])
			{
				umiestnenie[index] = game.turnId;
				ukoncene[game.turnId] = true;
				index++;
			}
		}
        
		game.turnId++;
	}
    
	system("clear");
	vykresli(squeres);
    
	for (int  i = 0; i < NUMBER_OF_PLAYERS; i++)
	{
		cout << i << ". miesto: hrac cislo: " << umiestnenie[i] << endl;
	}
    
}
