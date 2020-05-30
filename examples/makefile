CC=g++
CFLAGS=-std=c++11 -Wall -O3
INCLUDE_DIR=-I../

all: basic_usage typographic_example

basic_usage: basic_usage.cpp
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o $@
	
typographic_example: typographic_example.cpp ../CPPLOGGER.h
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o $@

clean:
	rm -rf basic_usage
	rm -rf typographic_example