/*
 * line_number.cpp
 *
 *  Created on: May 30, 2020
 *      Author: sajeeb
 */

#include <CPPLOGGER.h>

int main() {

	logger::info(logger::BOLD("-------------------------------------------------------------"));
	logger::info(logger::BOLD("Welcome to CPPLOGGER!"));
	logger::info(logger::BOLD("-------------------------------------------------------------"));

	logger::info("This message is printed with the line number.");

	logger::print_line(false);
	logger::info("This message is printed without the line number.");

}
