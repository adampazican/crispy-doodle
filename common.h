#pragma once

#include <stdint.h>

#define size(array) (int) (sizeof(array) / sizeof(array[0]))
#define HOUSE_SIZE 4
#define FIGURES_FOR_PLAYERS 4
#define GAME_SIZE 32

#define global_variable static

typedef int32_t i32;

enum Color {
	WHITE,
	RED,
	BLUE,
	YELLOW,
	GREEN
};

enum FigureState {
	IN_HOUSE,
	IN_GAME,
	IN_FINISH
};

enum GameState {
	WAITING,
	PLAYING,
	FINISHED
};

struct Figurine {
	FigureState figurineState;
	int position;
};

struct Player {
	int playerId;
	bool playingHand;
	Figurine figurines[4];
};

struct Game {
	GameState gameState;
	Player players[4];
	int turnId;
	int numberOfPlayers;
	int maxNumberOfPlayers;
    int winnerId;
};

struct Coordinates
{
    int x,y;
};



bool allInFinish(Player player);
Coordinates getSquareInGame(int possition,int playerId);