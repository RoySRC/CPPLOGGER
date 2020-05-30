/*
 * get_type.cpp
 *
 *  Created on: May 30, 2020
 *      Author: sajeeb
 */

#include <CPPLOGGER.h>

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

int main(int argc, char** argv) {

	sample_class c;
	c.print();
	logger::info("c: %s", logger::get_type(c));


	int a = 10;
	logger::info("type of a: %s", logger::get_type(a));
}
