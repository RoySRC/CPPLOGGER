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
		logger::info("print method in class: %s", logger::RED(logger::get_type(this)));
	}
};

void print_thread_id(uint id) {
	logger::info("%s: %03d", logger::RED("Message from thread"), id);
	logger::info("Message from thread: %03d", id);
}

void logger_thread_test() {
	const uint n_threads = 200;
	std::thread threads[n_threads];
	for (uint8_t i=0; i<n_threads; ++i)
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

	logger::info("%s", logger::RED("This is a red text"));
	logger::info(logger::RED("This is another red text"));

	logger::info(logger::BOLD(logger::RED("This is a bold red text.")));
	logger::info("%s", logger::BOLD(logger::RED("This is another bold red text.")));
}
