/*
 * get_type.cpp
 *
 *  Created on: May 30, 2020
 *      Author: sajeeb
 */

#include <CPPLOGGER.h>

/**
 * The following should always be called only in the main translation unit.
 * This is to default initialize the logger flags.
 */
logger_init();

class sample_class {
public:
	sample_class() {}
	void print() {
		cpplogger_info("print method in class: %s", cpplogger_red(logger::get_type(this)));
	}
};

int main(int argc, char** argv) {

	sample_class c;
	c.print();
	cpplogger_info("c: %s", logger::get_type(c));


	int a = 10;
	cpplogger_info("type of a: %s", logger::get_type(a));
}
