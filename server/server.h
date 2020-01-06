#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include "definitions.h"
#include "../common.h"

struct Request{
    i32 fileDescriptor;
    Player player;
    Game* game;
};

void* handle_request(Request* request);