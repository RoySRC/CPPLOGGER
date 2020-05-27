/*
 * main.cpp
 *
 *  Created on: May 27, 2020
 *      Author: sajeeb
 */

#include <CPPLOGGER.h>

int main(int argc, char** argv) {

	logger::error("This is a sample error message.");
	logger::error("%s%d", logger::RED("This is a non-zero exit status: ").c_str(), 255);

	logger::info(logger::BLUE("This is a blue info. message").c_str());
	logger::info(logger::UNDERLINE("This is an underlined line.").c_str());
	logger::info(logger::BOLD("This is a bold text.").c_str());
	logger::info(logger::BOLD(logger::UNDERLINE("This is a bold underlined text.")).c_str());
}
