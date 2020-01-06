
#include <iostream>
#include <time.h>
#include "../common.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

using namespace std;

struct Square {
	char character;
	Color color;
};

Square squares[11][11];
int numberOfPlayers;

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

void draw(Game game) 
{
	for (int  i = 0; i < numberOfPlayers; i++)
	{
		for (int j = 0; j < FIGURES_FOR_PLAYERS; j++)
		{
			FigureState state = game.players[i].figurines[j].figurineState;
			int possition = game.players[i].figurines[j].position;
			Color color = getColorByTurnId(i);
			struct Coordinates poss;
            
			if (game.players[i].figurines[j].position != 0 && state == FigureState::IN_HOUSE)
			{
				poss = getSquareInHouse(possition,i);
                squares[poss.y][poss.x].character = '0' + (j + 1);
                squares[poss.y][poss.x].color = color;
			}

			else if (state != FigureState::IN_HOUSE )
            {
                poss = state == FigureState::IN_GAME ? getSquareInGame(possition,i) : getSquareInFinish(possition,i);
                squares[poss.y][poss.x].character = '0' + (j + 1);
                squares[poss.y][poss.x].color = color;
            }
			
		}		
	}

	cout << "turnId: " <<  game.turnId << endl;
	
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
			cout << ' ';
			switch (squares[i][j].color)
			{
                case Color::BLUE:
				cout << "\033[1;34m"<< squares[i][j].character  << "\033[0m";
				break;
                case Color::GREEN:
				cout << "\033[1;32m" << squares[i][j].character << "\033[0m";
				break;
                case Color::YELLOW:
				cout << "\033[1;33m" << squares[i][j].character << "\033[0m";
				break;
                case Color::RED:
				cout << "\033[1;31m" << squares[i][j].character << "\033[0m";
				break;
                case Color::WHITE:
				cout << squares[i][j].character;
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

void PressEnterToContinue(int number)
{
    if (number == 0)
		cout << "Press ENTER to roll. ";
	else if (number == 1)
		cout << "Press ENTER to contunie in turn.";
	else 
		cout << "Press ENTER to finsih turn.";

    cin.clear();
    cin.get();
}

void clearBoard() {
    
    for (int  i = 0; i < 11; i++)
	{
		for (int  j = 0; j < 11; j++)
		{
			
			if (i > 3 && i < 7)				//Hracie policka
			{
				if (i == 5 && j == 5)
				{
					squares[i][j].character = ' ';
					squares[i][j].color = Color::WHITE;
				}
				else
				{
					squares[i][j].character = 'O';
					squares[i][j].color = Color::WHITE;
				}
			}
			else if (j > 3 && j < 7)				//Hracie policka
			{
				squares[i][j].character = 'O';
				squares[i][j].color = Color::WHITE;
			}
			else if (i < 2 && j < 2)				//IN_HOUSE_POLICKA
			{
				squares[i][j].character = 'O';
				squares[i][j].color = Color::RED;
			}
			else if (i > 8 && j < 2)
			{
				squares[i][j].character = 'O';
				squares[i][j].color = Color::BLUE;
			}
			else if (i < 2 && j >  8)
			{
				squares[i][j].character = 'O';
				squares[i][j].color = Color::GREEN;
			}
			else if (i > 8 && j > 8)
			{
				squares[i][j].character = 'O';
				squares[i][j].color = Color::YELLOW;
			}
			else			// prazdne policka
			{
				squares[i][j].character = ' ';
				squares[i][j].color = Color::WHITE;
			}
            //INFINISH policka
			if (i == 5 && j > 0 && j < 5)				
				squares[i][j].color = Color::BLUE;
			if (i == 5 && j > 5 && j < 10)
				squares[i][j].color = Color::GREEN;
			if (j == 5 && i > 0 && i < 5)
				squares[i][j].color = Color::RED;
			if (j == 5 && i > 5 && i < 10)
				squares[i][j].color = Color::YELLOW;
            
			if (j == 4 && i == 0)
			{
				squares[i][j].color = Color::RED;
				squares[i][j].character = '0';
			}
			if (j == 0 && i == 6)
			{
				squares[i][j].color = Color::BLUE;
				squares[i][j].character = '0';
			}
			if (j == 10 && i == 4)
			{
				squares[i][j].color = Color::GREEN;
				squares[i][j].character = '0';
			}
			if (j == 6 && i == 10)
			{
				squares[i][j].color = Color::YELLOW;
				squares[i][j].character = '0';
			}
		}
	}
}

Game communicateWithServer(Player player, char* hostname, char* port) 
{
	int sockfd;
	sockaddr_in serv_addr;
	hostent* server;
    
    Game game = {};
	    
	//Použijeme funkciu gethostbyname na získanie informácii o počítači, ktorého hostname je v prvom argumente.
	server = gethostbyname(hostname); 
	if (server == NULL)
	{
		//fprintf(stderr, "Error, no such host\n");
		return game;
	}
    
	//Vynulujeme a zinicializujeme adresu, na ktorú sa budeme pripájať.
	bzero((char*)&serv_addr, sizeof(serv_addr)); 
	serv_addr.sin_family = AF_INET;
	bcopy(
		(char*)server->h_addr,
		(char*)&serv_addr.sin_addr.s_addr,
		server->h_length
        );
	serv_addr.sin_port = htons(atoi(port));
    
	//Vytvoríme si socket v doméne AF_INET.
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd < 0)
	{
		perror("Error creating socket");
	}
    
	//Pripojíme sa na zadanú sieťovú adresu.
	if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)  
	{
        perror("Error connecting to socket");
	}	
    
	write(sockfd, &player, sizeof(player)); 
	read(sockfd, &game, sizeof(game)); 
    
	close(sockfd);
	return game;
}

int main(int argc, char *argv[])
{
	if(argc != 3)
    {
        cout << "Zly pocet parametrov" << endl;
        return 1;
    }
    
    char* hostname = argv[1];
    char* port = argv[2];
    
    srand(time(NULL));
    
	//pocet hracov v hre
	
    Player player = {};
	Game game = communicateWithServer(player, hostname, port);
	numberOfPlayers = game.numberOfPlayers;
    player = game.players[game.numberOfPlayers-1];
	system("clear");
	clearBoard();
	draw(game);
        
	while(1) {
		sleep(0.3);
		Game oldGame = game;
		game = communicateWithServer(player, hostname, port);
		numberOfPlayers = game.numberOfPlayers;
        
        if(game.gameState == GameState::FINISHED)
        {
            system("clear");
            clearBoard();
            draw(game);
            cout << "Vyhral hráč " << game.turnId << endl;
            break;
        }
        
		if(memcmp(&game, &oldGame, sizeof(Game)) == 0)
		{
			continue;
		}
        
		if(game.turnId == player.playerId && game.gameState == GameState::PLAYING)
		{
            
			system("clear");
			clearBoard();
			draw(game);
			
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
					PressEnterToContinue(0);
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
				PressEnterToContinue(0);
				number = getNumber();
				cout << "You got number " << number << endl;
				if (number == 6)
					numberOfSix++;
			}
			//ak hodi sestku hadze znova
			while (number == 6)
			{
				PressEnterToContinue(0);
				number = getNumber();
				cout << "You got number " << number << endl;
				if (number == 6)
					numberOfSix++;
			}
            
			PressEnterToContinue(1);

			while (numberOfSix >= 0)
			{
				system("clear");
				clearBoard();
				draw(game);
				
				if (game.turnId == 0)
					cout << "\033[1;31m" << "Red on the turn" << "\033[0m" << endl;
				else if (game.turnId == 1)
					cout << "\033[1;32m" << "Green on the turn." << "\033[0m" << endl;
				else if (game.turnId == 2)
					cout << "\033[1;33m" << "Yellow on the turn." << "\033[0m" << endl;
				else if (game.turnId == 3)
					cout << "\033[1;34m" << "Blue on the turn." << "\033[0m" << endl;
				
				int maxNumber = 0;
				
				if (numberOfSix > 0)
					maxNumber = 6;
				else
					maxNumber = number;
				
				if (allInHouse(game.players[game.turnId]) && maxNumber != 6)
				{
					cout << "you cant move." << endl;
					PressEnterToContinue(1);
				}
				else if (allInFinish(game.players[game.turnId]))
				{
					cout << "you cant move." << endl;
					PressEnterToContinue(1);
				}
				else if ( numberOfplayable(game.players[game.turnId],maxNumber) == 0 )
				{
					cout << "you cant move." << endl;
					PressEnterToContinue(1);
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
						game.players[game.turnId].figurines[figNumber].position = 0;
						game.players[game.turnId].figurines[figNumber].figurineState = FigureState::IN_GAME;
					}
					else if (game.players[game.turnId].figurines[figNumber].figurineState == FigureState::IN_GAME)
					{
						//overenie ci nejdem už do domceka
						if (game.players[game.turnId].figurines[figNumber].position + maxNumber > 37)
						{
							//presunie do ciela
							game.players[game.turnId].figurines[figNumber].figurineState = FigureState::IN_FINISH;
							game.players[game.turnId].figurines[figNumber].position += maxNumber - 37;
						} 
						else 
						{
						game.players[game.turnId].figurines[figNumber].position += maxNumber;
						}
					}
					else if (game.players[game.turnId].figurines[figNumber].figurineState == FigureState::IN_FINISH)
					{   
						game.players[game.turnId].figurines[figNumber].position += maxNumber;	
					}
					
					system("clear");
					clearBoard();
					draw(game);
				}
				numberOfSix--;
				player = game.players[game.turnId];
			}

			player.playingHand = true;
#if 0
            player.figurines[0].position = 1;
            player.figurines[0].figurineState = FigureState::IN_FINISH;
            player.figurines[1].position = 2;
            player.figurines[1].figurineState = FigureState::IN_FINISH;
            player.figurines[2].position = 3;
            player.figurines[2].figurineState = FigureState::IN_FINISH;
            player.figurines[3].position = 4;
            player.figurines[3].figurineState = FigureState::IN_FINISH;
#endif            
			game = communicateWithServer(player, hostname, port);
			player.playingHand = false;
		}
        
		system("clear");
		clearBoard();
		draw(game);
	}
}
