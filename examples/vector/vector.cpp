/*
 * vector.cpp
 *
 *  Created on: May 31, 2020
 *      Author: sajeeb
 */

#include "vector.h"

vector::vector() {}

vector::~vector() {
	for (size_t i=0; i<size; ++i) delete e[i];
	delete[] e;
	e = NULL;
}

void vector::add(element* __e) {
	if (size < capacity) {
		e[size] = __e;
		size += 1;

	} else {
		cpplogger_info("Resizing vector.");
		capacity += 10;
		element** tmp = new element* [capacity];
		for (size_t i=0; i<size; ++i) {
			tmp[i] = e[i];
		}
		delete[] e;
		e = tmp;
		add(__e);
		cpplogger_info("capacity: %ld", capacity);
	}
}
