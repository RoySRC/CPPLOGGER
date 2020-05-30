CC=g++
CFLAGS=-std=c++11 -Wall -O3
INCLUDE_DIR=-I../

all: basic_usage typographic_example \
	disable_logging time_stamp \
	multi_thread print_thread_id log_type\
	file_name line_number no_file_line

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
	
print_thread_id: print_thread_id.cpp
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o $@ -lpthread
	
log_type: log_type.cpp
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o $@
	
file_name: file_name.cpp
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o $@
	
line_number: line_number.cpp
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o $@
	
no_file_line: no_file_line.cpp
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o $@

clean:
	rm -rf basic_usage
	rm -rf typographic_example