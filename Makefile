BUILD_DIR=build
COMMON_FLAGS=-std=c++11 -Wall -pthread -Wno-unused-result
DEBUG_FLAGS=-O0 -ggdb
RELEASE_FLAGS=-Werror -O3
DEPSS=server/server.h common.h
UNITSS=server/linux_main.cpp server/server.cpp common.cpp
DEPSC=common.h
UNITSC=client/clovece.cpp common.cpp

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