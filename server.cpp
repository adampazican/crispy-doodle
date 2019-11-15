#include <string>
#include "server.h"

void handle_request(char* incomingBuffer, i32 incomingBufferLength,
                    char* outgoingBuffer, i32 outgoingBufferLength) 
{
    std::string incoming(incomingBuffer, incomingBufferLength);
    /*
    String request = {};
    request.data = incomingBuffer;
    request.length = incomingBufferLength;
    
    
    String line = {};
    line.data = incomingBuffer; //memcopy ? 
    
    i32 index = 0; 
    while(index < incomingBufferLength && incomingBuffer[index] != '\n')
        index++;
    line.length = index;
    
    
    String word = {};
    word.data = incomingBuffer;
    index = 0;
    while(index < incomingBufferLength && incomingBuffer[index] != ' ')
        index++;
    word.length = index;    
    
    String url = {};
    url.data = incomingBuffer + word.length + 1;
    index = 0;
    while(index < incomingBufferLength && url.data[index] != ' ')
        index++;
    url.length = index;
    
    if(word == "GET" && url == "/") 
    {
        printf("get request");
    }
    */
    //spawn a thread, make copy of buffer, handle request, alloc response...
    //strcpy(outgoingBuffer, "HTTP/1.1 200 OK \nDate: Mon, 27 Jul 2009 12:28:53 GMT \nServer: Apache/2.2.14 (Win32) \nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT \nContent-Length: 88 \nContent-Type: text/html \nConnection: Closed");
    int code = 200;
    sprintf(
        outgoingBuffer,
        "HTTP/1.1 %d OK\n"
        "Content-Type: text/html\n"
        "\n"
        "<html>"
        "<head>"
        "<title>Error code %d</title>"
        "</head>"
        "<body>"
        "<h1>Error code %d</h1>"
        "</body>"
        "</html>",
        code, code, code);
}

//void getFunc(request, response){
//    response->contentType = ("Application/json");
//}

/*
TODO:
- contact mysql database
- spawn a thread for every request
- validator
- toJson
- jwt token generation
- cache static files into memory
- https
- react frontend
- hashing algorithm + salt 
- 3 db entity, normalizacia
- CRUD operacie, použivaju sa všetky entity
*/