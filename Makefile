CC=g++
CFLAGS=-std=c++14 -Wall
INCLUDE_DIRS=-I.
LIBS=-lpthread

all: benchmark

benchmark: benchmark.cpp *.h
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $< -o $@ $(LIBS)
	
	
clean:
	rm -rf ./*.o ./benchmark