/*
 * element.cpp
 *
 *  Created on: May 31, 2020
 *      Author: sajeeb
 */

#include "element.h"

element::element() {
	logger::info("in empty constructor.");
}

element::element(int data) {
	logger::info("creating element with data: %d", data);
	this->data = data;
}

element::~element() {}