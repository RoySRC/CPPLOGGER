CC=g++
CFLAGS=-std=c++11 -Wall -O3
INCLUDE_DIR=-I../

all: basic_usage typographic_example disable_logging time_stamp multi_thread

basic_usage: basic_usage.cpp
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o $@
	
typographic_example: typographic_example.cpp
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o $@

disable_logging: disable_logging.cpp
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o $@

time_stamp: time_stamp.cpp
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o $@
	
multi_thread: multi_thread.cpp
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o $@ -lpthread

clean:
	rm -rf basic_usage
	rm -rf typographic_example