/*
 * typographic_example.cpp
 *
 *  Created on: May 29, 2020
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

	cpplogger_info(cpplogger_red("This is a red text"));
	cpplogger_info(
			cpplogger_red(
					cpplogger_italic("This is a red italic text.")
			)
	);
	cpplogger_info(cpplogger_bold(cpplogger_red("This is a red bold text")));
	cpplogger_info(cpplogger_red(cpplogger_bold(cpplogger_italic("This is a red bold italic text"))));


	cpplogger_info("%s: %d",
			cpplogger_bold(cpplogger_red("This is a red bold text with argument")),
			12);

	cpplogger_info("%s: %s",
			cpplogger_bold(cpplogger_red("This is a red bold text with green argument")),
			cpplogger_green("12"));

	cpplogger_info("%s: %s",
			cpplogger_bold(cpplogger_red("This is a red bold text with green bold argument")),
			cpplogger_bold(cpplogger_green("12")));

	cpplogger_info(cpplogger_underline("%s: %s"),
			cpplogger_bold(cpplogger_red("This is a red bold text with green bold argument")),
			cpplogger_bold(cpplogger_green("12")));
}
