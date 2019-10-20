#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#define internal static
#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))
#define Assert(Expression) if(!(Expression)) {*(volatile int *)0 = 0;}
#define CTAssert(Expr) static_assert(Expr, "Assertion failed: " #Expr)


typedef float r32;
typedef double r64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef i32 bool32;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#include "server.cpp"

int main() {
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(!serverFd) {
        printf("error\n");
    }

    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3000);
    
    int opt = 1;
    if(!setsockopt(serverFd, IPPROTO_TCP, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        printf("error with setsockopt: %s", strerror(errno));
    }

    if((bind(serverFd, (sockaddr*) &addr, sizeof(addr))))
    {
        printf("%d port unavailable\n", ntohs(addr.sin_port));
        addr.sin_port = htons(3001);
        if((bind(serverFd, (sockaddr*) &addr, sizeof(addr)))){
            printf("error %s\n", strerror(errno));
            return 0;
        }    
    }

    if((listen(serverFd, 69)))
    {
        printf("error3%s\n", strerror(errno));
    }

    char incomingBuffer[1000] = {}; //TODO: make dynamic buffers and realloc if not long enough
    char outgoingBuffer[1000] = {};

    while(1) {
        sockaddr_in clientAddr = {};
        socklen_t clientAddrLen = 0;
        int fd = accept(serverFd, (sockaddr*) &clientAddr, &clientAddrLen);
        assert(clientAddrLen == sizeof(clientAddr));

        if(!fd) {
            printf("error4\n");
        }
        else if (fd > 0){
            if(!read(fd, incomingBuffer, sizeof(incomingBuffer))){
                printf("error reading data: %s", strerror(errno));
                close(fd);
                continue;
            }
            
            handle_request(incomingBuffer, sizeof(incomingBuffer), outgoingBuffer, sizeof(outgoingBuffer));
            
            if(!send(fd, outgoingBuffer, strlen(outgoingBuffer), 0)){
                printf("error writing data: %s", strerror(errno));
            }
        }

        if(close(fd) < 0){
            printf("error closing connection\n");
        }
    }
    
    close(serverFd);
    return 0;
}