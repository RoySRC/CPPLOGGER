/*
 * file_name.cpp
 *
 *  Created on: May 30, 2020
 *      Author: sajeeb
 */

#include <CPPLOGGER.h>

int main() {

	logger::info(logger::BOLD("-------------------------------------------------------------"));
	logger::info(logger::BOLD("Welcome to CPPLOGGER!"));
	logger::info(logger::BOLD("-------------------------------------------------------------"));

	logger::info("This message is printed with the filename.");

	logger::print_file = false;
	logger::info("This message is printed without printing the filename.");

}
