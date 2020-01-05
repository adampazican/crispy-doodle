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

#define size(array) (int) (sizeof(array) / sizeof(array[0]))
#define HOUSE_SIZE 4

static Coordinates getSquareInGame(int possition,int playerId) {
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