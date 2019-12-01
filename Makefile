BUILD_DIR=build
COMMON_FLAGS=-std=c++11 -Wall -o build/server
DEBUG_FLAGS=-O0 -ggdb -fsanitize=address -fno-omit-frame-pointer -DDEBUG_BUILD=1
RELEASE_FLAGS=-Werror -O3
DEPS=server/server.h
UNITS=server/linux_main.cpp server/server.cpp

.DEFAULT_GOAL := all

clean:
	rm build/*

debug: $(DEPS) 
	g++ $(DEBUG_FLAGS) $(COMMON_FLAGS) $(UNITS)

release: $(DEPS)
	g++ $(RELEASE_FLAGS) $(COMMON_FLAGS) $(UNITS)

all: release
	

$(shell mkdir -p $(BUILD_DIR))