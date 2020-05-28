/*
 * main.cpp
 *
 *  Created on: May 27, 2020
 *      Author: sajeeb
 */

#include <CPPLOGGER.h>
#include <thread>

class sample_class {
public:
	sample_class() {}
	void print() {
		logger::info("print method in class: %s", logger::get_type(this));
	}
};

void print_thread_id(uint id) {
	logger::info("Message from thread: %d", id);
	logger::info("Message from thread: %d", id);
}

void logger_thread_test() {
	std::thread threads[10];
	for (uint8_t i=0; i<10; ++i)
		threads[i] = std::thread(print_thread_id, i+1);
	for (std::thread& t : threads) t.join();
}

int main(int argc, char** argv) {

	logger_thread_test();

	sample_class c;
	c.print();
	logger::info("c: %s", logger::get_type(c));


	int a = 10;
	logger::info("type of a: %s", logger::get_type(a));
}
