#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

#include "definitions.h"
#include "server.h"

int main(int argc, char *argv[]) {
    if(argc != 2)
    {
        printf("Zly pocet parametrov\n");
        return 1;
    }
    
    char* port = argv[1];
    
    printf("server initialization!!\n");
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(!serverFd) {
        printf("error\n");
    }
    
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    
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
    
    Game game = {};
    game.maxNumberOfPlayers = 2;
    game.numberOfPlayers = 0;
    
    while(1) {
        sockaddr_in clientAddr = {};
        socklen_t clientAddrLen = 0;
        i32 fd = accept(serverFd, (sockaddr*) &clientAddr, &clientAddrLen);
        assert(clientAddrLen == sizeof(clientAddr));
        
        if(!fd) {
            printf("error4\n");
        }
        else if (fd > 0){
            Request* request = (Request*) malloc(sizeof(Request));
            
            if(!read(fd, &request->player, sizeof(Player))){
                printf("error reading data: %d", errno);
                close(fd);
                continue;
            }
            
            request->fileDescriptor = fd;
            request->game = &game;
            
            //TODO: create new thread for handle_request 
            //pass buffers to handle_request but it must clean after itself
            
            handle_request(request);
        }
    }
    
    close(serverFd);
    return 0;
}