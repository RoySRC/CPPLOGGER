CC=g++
CFLAGS=-std=c++14 -Wall -O3
INCLUDE_DIRS=-I.

all: main.cpp
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $< -o main
	
clean:
	rm -rf ./*.o ./main