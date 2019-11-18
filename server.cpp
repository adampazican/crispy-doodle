#include "server.h"

enum FigurineState {
    IN_HOUSE,
    IN_GAME,
    IN_FINISH
};

struct __attribute__((packed)) Figurine {
    FigurineState figurineState;
    i32 position;
};

struct __attribute__((packed)) Player { //TODO: co ak hodi 6 a ma dalsi tah
    i32 playerId;
    Figurine figurines[4];
};

enum GameState {
    WAITING,
    PLAYING
};

struct __attribute__((packed)) Game {
    //TODO: error maybe? 
    GameState gameState;
    Player players[4];
    i32 playerTurnId;
};


void handle_request(i32 fileDescriptor, char* incomingBuffer, i32 incomingBufferLength, char* outgoingBuffer, i32 outgoingBufferLength) 
{
    Game game = {
        .gameState = WAITING,
        .playerTurnId = 1
    };
    
    game.players[0].playerId = 2;
    
    if(sizeof(game) > outgoingBufferLength) {
        //TODO: error for now, maybe realloc buffer to be bigger
        return;
    }
    
    memcpy(outgoingBuffer, &game, sizeof(game));
    
    if(!send(fileDescriptor, outgoingBuffer, outgoingBufferLength, 0)){
        printf("error writing data: %d", errno);
    }
    
    if(close(fileDescriptor) < 0){
        printf("error closing connection\n");
    }
    
    
    free(incomingBuffer);
}