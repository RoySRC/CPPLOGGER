/*
 * no_file_line.cpp
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

	cpplogger_info("This message has both line number and file");

	cpplogger_print_file(false);
	cpplogger_print_line(false);
	cpplogger_info("This message does not have either a file or a line number.");

}
