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

struct Array{
    u32 size;
    u32 capacity; 
    char data[];
};

#define array_alloc(type, size) arrayAlloc_(sizeof(type), size)

Array* arrayAlloc_(u32 length, u32 size) {
    Array* result = (Array*) malloc(sizeof(u32) * 2 + size*length);
    result->size = 0;
    result->capacity = size;
    return result;
}

#define array_realloc(array, type, size) arrayRealloc_(array, sizeof(type), size) 

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

int main() {
#if DEBUG_BUILD
    if(signal(SIGINT, sig_handler) == SIG_ERR){
        printf("cant catch signal ctrl+c");
    }
#endif
    
    printf("server initialization!!\n");
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
        printf("%d port unavailable, attempting port 3001\n", ntohs(addr.sin_port));
        addr.sin_port = htons(3001);
        if((bind(serverFd, (sockaddr*) &addr, sizeof(addr)))){
            printf("error initializing server %d\n", errno);
            return 0;
        }
        printf("Server started at port %d\n", ntohs(addr.sin_port));
    }
    
    if((listen(serverFd, 69)))
    {
        printf("error3%d\n", errno);
    }
    
    while(isRunning) {
        sockaddr_in clientAddr = {};
        socklen_t clientAddrLen = 0;
        i32 fd = accept(serverFd, (sockaddr*) &clientAddr, &clientAddrLen);
        assert(clientAddrLen == sizeof(clientAddr));
        
        if(!fd) {
            printf("error4\n");
        }
        else if (fd > 0){
            char* incomingBuffer = (char*) malloc(2000);
            i32 incomingBufferLength = 1000;
            char* outgoingBuffer = &incomingBuffer[1000]; //TODO: make request struct
            i32 outgoingBufferLength = 1000;
            
            if(!read(fd, incomingBuffer, incomingBufferLength)){
                printf("error reading data: %d", errno);
                close(fd);
                continue;
            }
            
            //TODO: create new thread for handle_request 
            //pass buffers to handle_request but it must clean after itself
            
            handle_request(fd, incomingBuffer, incomingBufferLength, outgoingBuffer, outgoingBufferLength);
        }
    }
    
    close(serverFd);
    return 0;
}