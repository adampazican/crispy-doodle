BUILD_DIR=build
COMMON_FLAGS=-std=c++11 -Wall
DEBUG_FLAGS=-O0 -ggdb -fsanitize=address -fno-omit-frame-pointer -DDEBUG_BUILD=1
RELEASE_FLAGS=-Werror -O3
DEPSS=server/server.h common.h
UNITSS=server/linux_main.cpp server/server.cpp
DEPSC=common.h
UNITSC=client/clovece.cpp

.DEFAULT_GOAL := all

clean:
	rm build/*

debugs: $(DEPSS) 
	g++ $(DEBUG_FLAGS) $(COMMON_FLAGS) $(UNITSS) -o build/server

releases: $(DEPSS)
	g++ $(RELEASE_FLAGS) $(COMMON_FLAGS) $(UNITSS) -o build/server

debugc: $(DEPSC) 
	g++ $(DEBUG_FLAGS) $(COMMON_FLAGS) $(UNITSC) -o build/client

releasec: $(DEPSC)
	g++ $(RELEASE_FLAGS) $(COMMON_FLAGS) $(UNITSC) -o build/client

debug: debugs debugc
release: releases releasec
all: release
	

$(shell mkdir -p $(BUILD_DIR))