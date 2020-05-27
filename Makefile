CC=g++
CFLAGS=-std=c++14 -Wall -O3
INCLUDE_DIRS=-I.
LIBS=-lpthread

all: main.cpp
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $< -o main $(LIBS)
	
clean:
	rm -rf ./*.o ./main