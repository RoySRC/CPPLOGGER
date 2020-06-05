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
	no_file_line\
	verbosity
	
run_all: all
	@python3 ./run_all.py

make_bin_dir:
	mkdir -p ./bin

basic_usage: basic_usage.cpp ../*.h
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o ./bin/$@
	
typographic_example: typographic_example.cpp ../*.h
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o ./bin/$@

disable_logging: disable_logging.cpp ../*.h
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o ./bin/$@

time_stamp: time_stamp.cpp ../*.h
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o ./bin/$@
	
multi_thread: multi_thread.cpp ../*.h
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o ./bin/$@ -lpthread
	
print_thread_id: print_thread_id.cpp ../*.h
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o ./bin/$@ -lpthread
	
log_type: log_type.cpp ../*.h
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o ./bin/$@
	
file_name: file_name.cpp ../*.h
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o ./bin/$@
	
line_number: line_number.cpp ../*.h
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o ./bin/$@
	
no_file_line: no_file_line.cpp ../*.h
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o ./bin/$@

verbosity: verbosity.cpp ../*.h
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $< -o ./bin/$@

clean:
	rm -rf ./bin/