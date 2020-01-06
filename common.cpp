#include "common.h"

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