#pragma once

#include <stdio.h>
#include "definitions.h"

void handle_request(char* incomingBuffer, i32 incomingBufferLength,
                    char* outgoingBuffer, i32 outgoingBufferLength);