CC=g++
CFLAGS=-std=c++14 -Wall -O3
INCLUDE_DIRS=-I.
LIBS=-lpthread

all: benchmark


benchmark: benchmark.cpp CPPLOGGER.h
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $< -o $@ $(LIBS)
	
	
clean:
	rm -rf ./*.o ./benchmark