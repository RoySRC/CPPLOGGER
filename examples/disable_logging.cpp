/*
 * disable_logging.cpp
 *
 *  Created on: May 30, 2020
 *      Author: sajeeb
 */

#include <CPPLOGGER.h>

int main() {

	logger::info(logger::BOLD("-------------------------------------------------------------"));
	logger::info(logger::BOLD("Welcome to CPPLOGGER!"));
	logger::info(logger::BOLD("-------------------------------------------------------------"));

	logger::info("This is before disabling global logging.");

	logger::disable();
	logger::info("This should not be printed.");

	logger::enable();
	logger::info("This is after enabling global logging.");

}
