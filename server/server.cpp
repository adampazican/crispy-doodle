#include "server.h"

global_variable Player zeroPlayer = {}; 
#define FIGURES_FOR_PLAYERS 4

void handle_turn(Game* game) 
{
    for (int i = 0; i < game->maxNumberOfPlayers; i++)
    {
        for (int j = 0; j < FIGURES_FOR_PLAYERS; j++)
        {
            if (game->turnId != i && game->players[i].figurines[j].figurineState == FigureState::IN_GAME)
            {
                Coordinates poss = getSquareInGame(game->players[game->turnId].figurines[j].position, game->turnId);
                Coordinates pom1 = getSquareInGame(game->players[i].figurines[j].position, i);

                if (poss.x == pom1.x && poss.y == pom1.y)
                {
                    game->players[i].figurines[j].position = j + 1;
                    game->players[i].figurines[j].figurineState = FigureState::IN_HOUSE;
                }
            }
        }
    }

}

void handle_request(i32 fileDescriptor, char* incomingBuffer, i32 incomingBufferLength, Game* game) 
{
    Player newPlayer = *(Player*) incomingBuffer;
    
    if(memcmp(&newPlayer, &zeroPlayer, sizeof(Player)) == 0 && game->gameState == GameState::WAITING && game->numberOfPlayers < game->maxNumberOfPlayers)
    {
        for(int i = 0; i < 4; i++) 
        {
            game->players[game->numberOfPlayers].figurines[i].figurineState = FigureState::IN_HOUSE;
            game->players[game->numberOfPlayers].figurines[i].position = i + 1;
            game->players[game->numberOfPlayers].playerId = game->numberOfPlayers;
        }
        
        game->numberOfPlayers++;
        
        if(game->numberOfPlayers == game->maxNumberOfPlayers)
        {
            game->gameState = GameState::PLAYING;
            game->turnId = 0;
        }
    }
    else if(newPlayer.playerId == game->turnId && game->gameState == GameState::PLAYING && newPlayer.playingHand)
    {
        game->players[newPlayer.playerId] = newPlayer;
        handle_turn(game);
        game->turnId++;
        game->turnId %= game->maxNumberOfPlayers;
        game->players[newPlayer.playerId].playingHand = false;
    }
    
    if(!send(fileDescriptor, game, sizeof(*game), 0)){
        printf("error writing data: %d", errno);
    }
    
    if(close(fileDescriptor) < 0){
        printf("error closing connection\n");
    }
    
    free(incomingBuffer);
}