#include "server.h"

global_variable Player zeroPlayer = {}; 

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
        game->turnId++;
        game->turnId %= game->maxNumberOfPlayers;
        
    }
    
    if(!send(fileDescriptor, game, sizeof(*game), 0)){
        printf("error writing data: %d", errno);
    }
    
    if(close(fileDescriptor) < 0){
        printf("error closing connection\n");
    }
    
    free(incomingBuffer);
}