clean:
	rm build/*

debug: server.h
	clang++ -std=c++11 -Wall -O0 -ggdb -fsanitize=address -fno-omit-frame-pointer -DDEBUG_BUILD=1 -o build/server linux_main.cpp server.cpp

build: server.h
	g++ -Wall -Werror -O3 -std=c++11 -o build/server linux_main.cpp server.cpp
	