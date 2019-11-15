#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>

#include "definitions.h"
#include "server.h"

struct GameState{
    i32 id;
    i32 pid;//TODO: this will serve for semaphore as indication whether process can alter this memory probably
    i32 x[4];
    i32 y[4];
};

struct Array{
    u32 size;
    u32 capacity; 
    char data[];
};

#define arrayAlloc(type, size) arrayAlloc_(sizeof(type), size)

Array* arrayAlloc_(u32 length, u32 size) {
    Array* result = (Array*) malloc(sizeof(u32) * 2 + size*length);
    result->size = 0;
    result->capacity = size;
    return result;
}

#define arrayRealloc(array, type, size) arrayRealloc_(array, sizeof(type), size) 

Array* arrayRealloc_(Array* array, u32 size, u32 newLength) {
    Array* result = (Array*) realloc(array, sizeof(u32) * 2 + size*newLength); 
    result->capacity = newLength;
    return result;
}

global_variable bool isRunning = true;
#if DEBUG_BUILD
void sig_handler(i32 sigNo)
{
    if(sigNo == SIGINT) {
        if (!isRunning) abort();
        isRunning = false;
    }
}
#endif

internal
i32 strlen(char* buffer) 
{
    i32 result = 0;
    
    while(buffer[result] != '\0') result++;
    return result;
}

int main() {
#if DEBUG_BUILD
    if(signal(SIGINT, sig_handler) == SIG_ERR){
        printf("cant catch signal ctrl+c");
    }
#endif
    
    Array* game_states = arrayAlloc(GameState, 4);
    game_states = arrayRealloc(game_states, GameState, 5);
    
    ((GameState*) game_states->data)[4] = GameState{
        .id = 10
    };
    ((GameState*) game_states->data)[1] = GameState{
        .id = 20
    };
    ((GameState*) game_states->data)[2] = GameState{
        .id = 30
    };
    
    printf("%d", game_states->size);
    printf("%d", ((GameState*) game_states->data)[0].id);
    
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(!serverFd) {
        printf("error\n");
    }
    
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3000);
    
    int opt = 1;
    if(!setsockopt(serverFd, IPPROTO_TCP, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        printf("error with setsockopt: %d", errno);
    }
    
    if((bind(serverFd, (sockaddr*) &addr, sizeof(addr))))
    {
        printf("%d port unavailable\n", ntohs(addr.sin_port));
        addr.sin_port = htons(3001);
        if((bind(serverFd, (sockaddr*) &addr, sizeof(addr)))){
            printf("error %d\n", errno);
            return 0;
        }    
    }
    
    if((listen(serverFd, 69)))
    {
        printf("error3%d\n", errno);
    }
    
    char incomingBuffer[1000] = {}; //TODO: make dynamic buffers and realloc if not long enough
    char outgoingBuffer[1000] = {};
    
    while(isRunning) {
        sockaddr_in clientAddr = {};
        socklen_t clientAddrLen = 0;
        int fd = accept(serverFd, (sockaddr*) &clientAddr, &clientAddrLen);
        assert(clientAddrLen == sizeof(clientAddr));
        
        if(!fd) {
            printf("error4\n");
        }
        else if (fd > 0){
            if(!read(fd, &incomingBuffer, sizeof(incomingBuffer))){
                printf("error reading data: %d", errno);
                close(fd);
                continue;
            }
            
            handle_request(incomingBuffer, sizeof(incomingBuffer), outgoingBuffer, sizeof(outgoingBuffer));
            
            if(!send(fd, outgoingBuffer, strlen(outgoingBuffer), 0)){
                printf("error writing data: %d", errno);
            }
        }
        
        if(close(fd) < 0){
            printf("error closing connection\n");
        }
    }
    
    
    
    close(serverFd);
    return 0;
}