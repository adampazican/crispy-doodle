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
    if(argc != 3)
    {
        printf("Zly pocet parametrov\n");
        return 1;
    }
    
    char* port = argv[1];
    char* pocetHracov = argv[2];
    
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
        addr.sin_port = htons(atoi(port)+1);
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
    game.numberOfPlayers = 0;
    
    if (atoi(pocetHracov) > 4 )
    {
        game.maxNumberOfPlayers = 4;
        printf("Number of players was set to 4.\n");
    } else if (atoi(pocetHracov) < 2)
    {
        game.maxNumberOfPlayers = 2;
        printf("Number of players was set to 2.\n");
    } else 
    {
        game.maxNumberOfPlayers = atoi(pocetHracov);
    }
    
    pthread_mutex_t mutex = {};
    pthread_mutex_init(&mutex, 0);
    
    while(1) {
        sockaddr_in clientAddr = {};
        socklen_t clientAddrLen = 0;
        i32 fd = accept(serverFd, (sockaddr*) &clientAddr, &clientAddrLen);
        if(fd == -1)
        {
            perror("accept failed");
            continue;
        }
        assert(clientAddrLen == sizeof(clientAddr));
        
        if(!fd) {
            printf("error4\n");
        }
        else if (fd > 0){
            Request* request = (Request*) malloc(sizeof(Request));
            pthread_t thread = {};
            pthread_attr_t thread_attr = {};
            
            pthread_attr_init(&thread_attr); pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
            
            if(!read(fd, &request->player, sizeof(Player))){
                printf("error reading data: %d", errno);
                close(fd);
                continue;
            }
            
            request->fileDescriptor = fd;
            request->game = &game;
            request->mutex = &mutex;
            
            if(pthread_create(&thread, &thread_attr, &handle_request, request))
            {
                perror("thread couldnt be created because of reason");
                assert(0);
            }
            
            pthread_attr_destroy(&thread_attr);
        }
    }
    
    close(serverFd);
    pthread_mutex_destroy(&mutex);
    return 0;
}