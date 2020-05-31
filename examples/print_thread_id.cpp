/*
 * print_thread_id.cpp
 *
 *  Created on: May 30, 2020
 *      Author: sajeeb
 */

#include <CPPLOGGER.h>

void print_thread(uint id) {
	logger::print_thread_id(true);
	logger::info("%s: %03d", logger::RED("Message from thread"), id);
	logger::info("Message from thread: %03d", id);
}

int main() {
	const uint n_threads = 200;
	std::thread threads[n_threads];
	for (uint8_t i=0; i<n_threads; ++i)
		threads[i] = std::thread(print_thread, i+1);
	for (std::thread& t : threads) t.join();
}
