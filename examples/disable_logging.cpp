/*
 * disable_logging.cpp
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

int main() {

	cpplogger_info(cpplogger_bold("-------------------------------------------------------------"));
	cpplogger_info(cpplogger_bold("Welcome to CPPLOGGER!"));
	cpplogger_info(cpplogger_bold("-------------------------------------------------------------"));

	cpplogger_info("This is before disabling global logging.");

	cpplogger_enable(false);
	cpplogger_info("This should not be printed.");

	cpplogger_enable(true);
	cpplogger_info("This is after enabling global logging.");

}
