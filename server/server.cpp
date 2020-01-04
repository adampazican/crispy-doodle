#include "server.h"

global_variable Game zeroGame = {}; 

void handle_request(i32 fileDescriptor, char* incomingBuffer, i32 incomingBufferLength, Game* game) 
{
    Game newGame = *(Game*) incomingBuffer;

    if(memcmp(&newGame, &zeroGame, sizeof(Game)) == 0 && game->gameState == GameState::WAITING && game->numberOfPlayers < game->maxNumberOfPlayers){
        game->players[game->numberOfPlayers].playerId = game->numberOfPlayers;

        for(int i = 0; i < 4; i++) 
        {
            game->players[game->numberOfPlayers].figurines[i].figurineState = FigureState::IN_HOUSE;
            game->players[game->numberOfPlayers].figurines[i].position = i + 1;
        } 
        (game->numberOfPlayers)++;

        if(game->numberOfPlayers == game->maxNumberOfPlayers)
        {
            game->gameState = GameState::PLAYING;
            game->turnId = 0;
        }

    }
    else if(game->turnId == newGame.turnId && game->gameState == GameState::PLAYING && memcmp(&newGame, &zeroGame, sizeof(Game)) != 0)
    {
        *game = newGame;
        game->turnId++;
        game->turnId %= 4;
    }
            
    if(!send(fileDescriptor, game, sizeof(*game), 0)){
        printf("error writing data: %d", errno);
    }
    
    if(close(fileDescriptor) < 0){
        printf("error closing connection\n");
    }
    
    
    free(incomingBuffer);
}