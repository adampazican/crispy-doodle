#include "server.h"

void handle_request(i32 fileDescriptor, char* incomingBuffer, i32 incomingBufferLength, char* outgoingBuffer, i32 outgoingBufferLength) 
{
    //Player newPLayer = *(Player*) incomingBuffer;
    
    Game game = {
        WAITING,
        1
    };
    
    game.players[0].playerId = 2;
    
    assert(sizeof(game) <= (u32)outgoingBufferLength);
    
    memcpy(outgoingBuffer, &game, sizeof(game));
    
    if(!send(fileDescriptor, outgoingBuffer, outgoingBufferLength, 0)){
        printf("error writing data: %d", errno);
    }
    
    if(close(fileDescriptor) < 0){
        printf("error closing connection\n");
    }
    
    
    free(incomingBuffer);
}