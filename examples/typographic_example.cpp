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

	logger::info(logger::BOLD("-------------------------------------------------------------"));
	logger::info(logger::BOLD("Welcome to CPPLOGGER!"));
	logger::info(logger::BOLD("-------------------------------------------------------------"));

	logger::info(logger::RED("This is a red text"));
	logger::info(
			logger::RED(
					logger::ITALIC("This is a red italic text.")
			)
	);
	logger::info(logger::BOLD(logger::RED("This is a red bold text")));
	logger::info(logger::RED(logger::BOLD(logger::ITALIC("This is a red bold italic text"))));


	logger::info("%s: %d",
			logger::BOLD(logger::RED("This is a red bold text with argument")),
			12);

	logger::info("%s: %s",
			logger::BOLD(logger::RED("This is a red bold text with green argument")),
			logger::GREEN("12"));

	logger::info("%s: %s",
			logger::BOLD(logger::RED("This is a red bold text with green bold argument")),
			logger::BOLD(logger::GREEN("12")));

	logger::info(logger::UNDERLINE("%s: %s"),
			logger::BOLD(logger::RED("This is a red bold text with green bold argument")),
			logger::BOLD(logger::GREEN("12")));
}
