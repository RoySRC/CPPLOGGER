/*
 * CPPLOGGER.h
 *
 *  Created on: May 27, 2020
 *      Author: sajeeb
 */

#ifndef CPPLOGGER_H_
#define CPPLOGGER_H_

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdarg.h>

using std::string;


#define info(fmt, ...) _info_ (__FILE__, __LINE__, fmt, ##__VA_ARGS__);
#define error(fmt, ...) _error_ (__FILE__, __LINE__, fmt, ##__VA_ARGS__);
#define warning(fmt, ...) _warning_ (__FILE__, __LINE__, fmt, ##__VA_ARGS__);


namespace logger {

	const string ANSI_RESET = "\u001B[0m";
	const string ANSI_BLACK = "\u001B[30m";
	const string ANSI_RED = "\u001B[31m";
	const string ANSI_GREEN = "\u001B[32m";
	const string ANSI_YELLOW = "\u001B[33m";
	const string ANSI_BLUE = "\u001B[34m";
	const string ANSI_PURPLE = "\u001B[35m";
	const string ANSI_CYAN = "\u001B[36m";
	const string ANSI_WHITE = "\u001B[37m";
	const string ANSI_BOLD = "\u001B[1m";
	const string ANSI_UNDERLINE = "\u001B[4m";

	string BLACK(string msg) {
		return ANSI_BLACK + msg + ANSI_RESET;
	}

	string RED(string msg) {
		return ANSI_RED + msg + ANSI_RESET;
	}

	string GREEN(string msg) {
		return ANSI_GREEN + msg + ANSI_RESET;
	}

	string YELLOW(string msg) {
		return ANSI_YELLOW + msg + ANSI_RESET;
	}

	string BLUE(string msg) {
		return ANSI_BLUE + msg + ANSI_RESET;
	}

	string PURPLE(string msg) {
		return ANSI_PURPLE + msg + ANSI_RESET;
	}

	string CYAN(string msg) {
		return ANSI_CYAN + msg + ANSI_RESET;
	}

	string WHITE(string msg) {
		return ANSI_WHITE + msg + ANSI_RESET;
	}

	string BOLD(string msg) {
		return ANSI_BOLD + msg + ANSI_RESET;
	}

	string UNDERLINE(string msg) {
		return ANSI_UNDERLINE + msg + ANSI_RESET;
	}

	void print(string type, const char* _file_, int line, const char* fmt, va_list& args) {
		printf("%s: %s:%d: ", type.c_str(), _file_, line);
		vfprintf(stdout, fmt, args);
		printf("\n");
		fflush(stdout);
		va_end(args);
	}

	void _info_(const char* _file_, int line, const char* fmt, ...) {
		va_list args; va_start(args, fmt);
		print(logger::GREEN("[INFO]"), _file_, line, fmt, args);
	}

	void _error_(const char* _file_, int line, const char* fmt, ...) {
		va_list args; va_start(args, fmt);
		print(logger::RED("[ERROR]"), _file_, line, fmt, args);
	}

	void _warning_(const char* _file_, int line, const char* fmt, ...) {
		va_list args; va_start(args, fmt);
		print(logger::BLUE("[WARN]"), _file_, line, fmt, args);
	}

	template<typename T>
	string get_classname(T c) {
		return "";
	}

}

#endif /* CPPLOGGER_H_ */
