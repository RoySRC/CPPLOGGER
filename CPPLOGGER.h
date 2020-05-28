/*
 * CPPLOGGER.h
 *
 *  Created on: May 27, 2020
 *      Author: sajeeb
 */

#ifndef CPPLOGGER_H_
#define CPPLOGGER_H_

#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <thread>
#include <mutex>
#include <cxxabi.h>

using std::string;
using std::mutex;

#define info(fmt, ...) _info_ (__FILE__, __LINE__, fmt, ##__VA_ARGS__);
#define error(fmt, ...) _error_ (__FILE__, __LINE__, fmt, ##__VA_ARGS__);
#define warning(fmt, ...) _warning_ (__FILE__, __LINE__, fmt, ##__VA_ARGS__);


namespace logger {

	mutex critical_section;

	bool print_timestamps = false;
	bool print_thread_id = false;

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
	const string ANSI_ITALIC = "\u001B[3m";

	/**
	 * The following set of functions take in a string object as argument and return a const char pointer
	 */
	const char* BLACK(string msg) {
		return (ANSI_BLACK + msg + ANSI_RESET).c_str();
	}

	const char* RED(string msg) {
		return (ANSI_RED + msg + ANSI_RESET).c_str();
	}

	const char* GREEN(string msg) {
		return (ANSI_GREEN + msg + ANSI_RESET).c_str();
	}

	const char* YELLOW(string msg) {
		return (ANSI_YELLOW + msg + ANSI_RESET).c_str();
	}

	const char* BLUE(string msg) {
		return (ANSI_BLUE + msg + ANSI_RESET).c_str();
	}

	const char* PURPLE(string msg) {
		return (ANSI_PURPLE + msg + ANSI_RESET).c_str();
	}

	const char* CYAN(string msg) {
		return (ANSI_CYAN + msg + ANSI_RESET).c_str();
	}

	const char* WHITE(string msg) {
		return (ANSI_WHITE + msg + ANSI_RESET).c_str();
	}

	const char* BOLD(string msg) {
		return (ANSI_BOLD + msg + ANSI_RESET).c_str();
	}

	const char* UNDERLINE(string msg) {
		return (ANSI_UNDERLINE + msg + ANSI_RESET).c_str();
	}

	const char* ITALIC(string msg) {
		return (ANSI_ITALIC + msg + ANSI_RESET).c_str();
	}

	/**
	 * The following set of functions are used when parameter and return types are the same
	 */
	template<typename T>
	T BLACK(T msg) {
		return (ANSI_BLACK + string(msg) + ANSI_RESET).c_str();
	}

	template<typename T>
	T RED(T msg) {
		return (ANSI_RED + string(msg) + ANSI_RESET).c_str();
	}

	template<typename T>
	T GREEN(T msg) {
		return (ANSI_GREEN + string(msg) + ANSI_RESET).c_str();
	}

	template<typename T>
	T YELLOW(T msg) {
		return (ANSI_YELLOW + string(msg) + ANSI_RESET).c_str();
	}

	template<typename T>
	T BLUE(T msg) {
		return (ANSI_BLUE + string(msg) + ANSI_RESET).c_str();
	}

	template<typename T>
	T PURPLE(T msg) {
		return (ANSI_PURPLE + string(msg) + ANSI_RESET).c_str();
	}

	template<typename T>
	T CYAN(T msg) {
		return (ANSI_CYAN + string(msg) + ANSI_RESET).c_str();
	}

	template<typename T>
	T WHITE(T msg) {
		return (ANSI_WHITE + string(msg) + ANSI_RESET).c_str();
	}

	template<typename T>
	T BOLD(T msg) {
		return (ANSI_BOLD + string(msg) + ANSI_RESET).c_str();
	}

	template<typename T>
	T UNDERLINE(T msg) {
		return (ANSI_UNDERLINE + string(msg) + ANSI_RESET).c_str();
	}

	template<typename T>
	T ITALIC(T msg) {
		return (ANSI_ITALIC + string(msg) + ANSI_RESET).c_str();
	}

	/**
	 * Wrapper function for printing logging information to screen
	 */
	void print(string type, const char* _file_, int line, const char* fmt, va_list& args) {
		std::stringstream ss; ss << std::this_thread::get_id();
		unsigned long long int id = std::stoull(ss.str());

		auto duration = std::chrono::system_clock::now().time_since_epoch();
		unsigned long int millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

		critical_section.lock();
		printf("%s", type.c_str());

		if (print_timestamps) printf("[%ld]", millis);
		if (print_thread_id) printf("[%lld]", id);

		printf(": %s:%d:: ", _file_, line);
		vfprintf(stdout, fmt, args);
		printf("\n");
		fflush(stdout);
		va_end(args);
		critical_section.unlock();
	}

	/**
	 *
	 */
	void _info_(const char* _file_, int line, const char* fmt, ...) {
		va_list args; va_start(args, fmt);
		print(GREEN("[INFO]"), _file_, line, fmt, args);
	}

	/**
	 *
	 */
	void _error_(const char* _file_, int line, const char* fmt, ...) {
		va_list args; va_start(args, fmt);
		print(RED("[ERROR]"), _file_, line, fmt, args);
	}

	/**
	 *
	 */
	void _warning_(const char* _file_, int line, const char* fmt, ...) {
		va_list args; va_start(args, fmt);
		print(BLUE("[WARN]"), _file_, line, fmt, args);
	}

	/**
	 * The following two functions are used to get the name of a type
	 */
	template<typename T>
	const char* get_type(T* c) {
		auto ptr = std::unique_ptr<char, decltype(& std::free)>{
			abi::__cxa_demangle(typeid(*c).name(), nullptr, nullptr, nullptr),
			std::free
		};
		return string(ptr.get()).c_str();
	}

	template<typename T>
	const char* get_type(T c) {
		auto ptr = std::unique_ptr<char, decltype(& std::free)>{
			abi::__cxa_demangle(typeid(c).name(), nullptr, nullptr, nullptr),
			std::free
		};
		return string(ptr.get()).c_str();
	}

}

#endif /* CPPLOGGER_H_ */
