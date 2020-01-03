#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "definitions.h"
#include "../common.h"

void handle_request(i32 fileDescriptor, char* incomingBuffer, i32 incomingBufferLength,
                    char* outgoingBuffer, i32 outgoingBufferLength);