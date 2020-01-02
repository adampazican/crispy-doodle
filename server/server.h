#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "definitions.h"

void handle_request(i32 fileDescriptor, char* incomingBuffer, i32 incomingBufferLength,
                    char* outgoingBuffer, i32 outgoingBufferLength);