/*
 * time_stamp.cpp
 *
 *  Created on: May 30, 2020
 *      Author: sajeeb
 */

#include <CPPLOGGER.h>

int main() {

	logger::info(logger::BOLD("-------------------------------------------------------------"));
	logger::info(logger::BOLD("Welcome to CPPLOGGER!"));
	logger::info(logger::BOLD("-------------------------------------------------------------"));

	logger::info("This is printed without a time stamp.");

	logger::print_timestamps = true;
	logger::info("This is printed with a time stamp.");

}