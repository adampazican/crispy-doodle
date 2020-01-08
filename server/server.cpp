#include "server.h"

global_variable Player zeroPlayer = {}; 

void handle_turn(Game* game) 
{
    for (int i = 0; i < game->maxNumberOfPlayers; i++)
    {
        if (game->turnId != i)
        {
            for (int j = 0; j < FIGURES_FOR_PLAYERS; j++)
            {
                if(game->players[i].figurines[j].figurineState == FigureState::IN_GAME)
                {
                    for (int k = 0; k < FIGURES_FOR_PLAYERS; k++)
                    {
                        Coordinates poss = getSquareInGame(game->players[game->turnId].figurines[k].position, game->turnId);
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
    }
    
}

void* handle_request(void* data) 
{
    Request* request = (Request*) data;
    
    pthread_mutex_lock(request->mutex);
    
    if(request->game->gameState == GameState::WAITING && 
       memcmp(&request->player, &zeroPlayer, sizeof(Player)) == 0 && 
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
    
    Game newGame = *(Game*)request->game;
    pthread_mutex_unlock(request->mutex);
    
    if(request->player.playerId == newGame.turnId && 
            newGame.gameState == GameState::PLAYING && 
            request->player.playingHand)
    {        
        
        newGame.players[request->player.playerId] = request->player;
        handle_turn(&newGame);
        newGame.players[request->player.playerId].playingHand = false;
        
        if(allInFinish(newGame.players[newGame.turnId]))
        {
            newGame.gameState = GameState::FINISHED;
            newGame.winnerId = newGame.turnId;
        }
        else
        {
            newGame.turnId++;
            newGame.turnId %= newGame.maxNumberOfPlayers;
        }
        
        pthread_mutex_lock(request->mutex);
        *(Game*)request->game = newGame;
        pthread_mutex_unlock(request->mutex);
    }
        
    if(!send(request->fileDescriptor, &newGame, sizeof(newGame), 0)){
        printf("error writing data: %d", errno);
    }
        
    if(close(request->fileDescriptor) < 0){
        printf("error closing connection\n");
    }
    
    free(request);
    
    return 0;
}
