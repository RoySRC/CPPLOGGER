/*
 * no_file_line.cpp
 *
 *  Created on: May 30, 2020
 *      Author: sajeeb
 */

#include <CPPLOGGER.h>

int main() {

	logger::info(logger::BOLD("-------------------------------------------------------------"));
	logger::info(logger::BOLD("Welcome to CPPLOGGER!"));
	logger::info(logger::BOLD("-------------------------------------------------------------"));

	logger::info("This message has both line number and file");

	logger::print_file = false;
	logger::print_line = false;
	logger::info("This message does not have either a file or a line number.");

}
