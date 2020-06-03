/*
 * main.cpp
 *
 *  Created on: May 31, 2020
 *      Author: sajeeb
 */

#include <CPPLOGGER.h>
#include <iostream>
#include <vector.h>

/**
 * The following should always be called only in the main translation unit.
 * This is to default initialize the logger flags.
 */
logger_init();

int main() {

	vector v;

	v.add(new element(12));

	logger_enable(false);

	v.add(new element(13));

}
