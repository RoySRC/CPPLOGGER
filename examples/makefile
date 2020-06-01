CC=g++
CFLAGS=-std=c++11 -Wall -O3
INCLUDE_DIR=-I../

all: \
	make_bin_dir\
	basic_usage\
	typographic_example \
	disable_logging\
	time_stamp \
	multi_thread\
	print_thread_id\
	log_type\
	file_name\
	line_number\
	no_file_line
	
run_all: all
	@python3 ./run_all.py

make_bin_dir:
	mkdir -p ./bin

basic_usage: basic_usage.cpp ../CPPLOGGER.h
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o ./bin/$@
	
typographic_example: typographic_example.cpp ../CPPLOGGER.h
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o ./bin/$@

disable_logging: disable_logging.cpp ../CPPLOGGER.h
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o ./bin/$@

time_stamp: time_stamp.cpp ../CPPLOGGER.h
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o ./bin/$@
	
multi_thread: multi_thread.cpp ../CPPLOGGER.h
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o ./bin/$@ -lpthread
	
print_thread_id: print_thread_id.cpp ../CPPLOGGER.h
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o ./bin/$@ -lpthread
	
log_type: log_type.cpp ../CPPLOGGER.h
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o ./bin/$@
	
file_name: file_name.cpp ../CPPLOGGER.h
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o ./bin/$@
	
line_number: line_number.cpp ../CPPLOGGER.h
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o ./bin/$@
	
no_file_line: no_file_line.cpp ../CPPLOGGER.h
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o ./bin/$@

clean:
	rm -rf ./bin/basic_usage
	rm -rf ./bin/typographic_example
	rm -rf ./bin/disable_logging
	rm -rf ./bin/time_stamp
	rm -rf ./bin/multi_thread
	rm -rf ./bin/print_thread_id
	rm -rf ./bin/log_type
	rm -rf ./bin/file_name
	rm -rf ./bin/line_number
	rm -rf ./bin/no_file_line