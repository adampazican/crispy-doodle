#include "server.h"

global_variable Player zeroPlayer = {}; 

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

void* handle_request(Request* request) 
{
    if(memcmp(&request->player, &zeroPlayer, sizeof(Player)) == 0 && 
       request->game->gameState == GameState::WAITING && 
       request->game->numberOfPlayers < request->game->maxNumberOfPlayers)
    {
        for(int i = 0; i < FIGURES_FOR_PLAYERS; i++) 
        {
            request->game->players[request->game->numberOfPlayers].figurines[i].figurineState = FigureState::IN_HOUSE;
            request->game->players[request->game->numberOfPlayers].figurines[i].position = i + 1;
            request->game->players[request->game->numberOfPlayers].playerId = request->game->numberOfPlayers;
        }
        
        request->game->numberOfPlayers++;
        
        if(request->game->numberOfPlayers == request->game->maxNumberOfPlayers)
        {
            request->game->gameState = GameState::PLAYING;
            request->game->turnId = 0;
        }
    }
    else if(request->player.playerId == request->game->turnId && 
            request->game->gameState == GameState::PLAYING && 
            request->player.playingHand)
    {
        request->game->players[request->player.playerId] = request->player;
        handle_turn(request->game);
        request->game->players[request->player.playerId].playingHand = false;
        
        if(allInFinish(request->game->players[request->game->turnId]))
        {
            request->game->gameState = GameState::FINISHED;
            request->game->winnerId = request->game->turnId;
        }
        else
        {
            request->game->turnId++;
            request->game->turnId %= request->game->maxNumberOfPlayers;
        }
    }
    
    if(!send(request->fileDescriptor, request->game, sizeof(*request->game), 0)){
        printf("error writing data: %d", errno);
    }
    
    if(close(request->fileDescriptor) < 0){
        printf("error closing connection\n");
    }
    
    free(request);
    
    return 0;
}
