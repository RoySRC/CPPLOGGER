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
#include <string.h>
#include <stdio.h>

using std::string;
using std::mutex;

#define __force_inline__ __attribute__((always_inline)) inline

/**
 * The following critical section macro is to make the logger thread safe
 */
#define CRITICAL_SECTION_CODE(code) 	\
			critical_section.lock(); 	\
			logger::code; 				\
			logger::critical_section.unlock();

/**
 * The following block is to wrap functions calls to the logger function inside a critical
 * section block for thread safety
 */
#define info(fmt, ...) CRITICAL_SECTION_CODE(_info_(__FILE__, __LINE__, fmt, ##__VA_ARGS__);)
#define error(fmt, ...) CRITICAL_SECTION_CODE(_error_(__FILE__, __LINE__, fmt, ##__VA_ARGS__);)
#define warning(fmt, ...) CRITICAL_SECTION_CODE(_warning_(__FILE__, __LINE__, fmt, ##__VA_ARGS__);)

#define BLACK(msg) 		_BLACK_(msg).c_str()
#define RED(msg)		_RED_(msg).c_str()
#define GREEN(msg) 		_GREEN_(msg).c_str()
#define YELLOW(msg) 	_YELLOW_(msg).c_str()
#define BLUE(msg) 		_BLUE_(msg).c_str()
#define PURPLE(msg) 	_PURPLE_(msg).c_str()
#define CYAN(msg) 		_CYAN_(msg).c_str()
#define WHITE(msg) 		_WHITE_(msg).c_str()
#define BOLD(msg) 		_BOLD_(msg).c_str()
#define UNDERLINE(msg) 	_UNDERLINE_(msg).c_str()
#define ITALIC(msg) 	_ITALIC_(msg).c_str()

namespace logger {

	/**
	 *  Mutex lock for critical section. here critical section
	 *  refers to the block of code responsible for printing
	 *  logging information to screen.
	 */
	mutex critical_section;

	bool print_timestamps = false;
	bool print_thread_id = false;

	/**
	 * Setting the following to false will not print "[INFO]: ", "[ERROR]: ", "[WARN]: "
	 * before log messages.
	 */
	bool print_log_type = true;

	bool print_file = true;
	bool print_line = true;

	/**
	 * setting the following to false will disable global logging.
	 */
	bool _enable = true;

	/**
	 * Add support for colors for versions older than c++11. This is for backwards compatibility
	 */
	#if __cplusplus >= 201103L
	#define COLOR_UNICODE "\u001b"
	#else
	#define COLOR_UNICODE "\x1b"
	#endif

	/**
	 * Define the ANSI color codes
	 */
	#define ANSI_RESET		COLOR_UNICODE	"[00m"
	#define ANSI_BLACK		COLOR_UNICODE	"[30m"
	#define ANSI_RED		COLOR_UNICODE	"[31m"
	#define ANSI_GREEN		COLOR_UNICODE	"[32m"
	#define ANSI_YELLOW		COLOR_UNICODE	"[33m"
	#define ANSI_BLUE		COLOR_UNICODE	"[34m"
	#define ANSI_PURPLE		COLOR_UNICODE	"[35m"
	#define ANSI_CYAN		COLOR_UNICODE	"[36m"
	#define ANSI_WHITE		COLOR_UNICODE	"[37m"
	#define ANSI_BOLD		COLOR_UNICODE	"[01m"
	#define ANSI_UNDERLINE	COLOR_UNICODE	"[04m"
	#define ANSI_ITALIC		COLOR_UNICODE	"[03m"

	/**
	 * The following function disables global logging
	 */
	__force_inline__
	void disable() {
		_enable = false;
	}

	/**
	 * The following function enables global logging
	 */
	__force_inline__
	void enable() {
		_enable = true;
	}

	__force_inline__
	string COLOR(const char* color, string msg) {
		return color+msg+ANSI_RESET;
	}

	/**
	 * The following set of macros take in a string object as argument and return a constant char pointer
	 */
	__force_inline__ string _BLACK_(string msg)		{	return COLOR(ANSI_BLACK, msg);		}
	__force_inline__ string _RED_(string msg)		{	return COLOR(ANSI_RED, msg);		}
	__force_inline__ string _GREEN_(string msg)		{	return COLOR(ANSI_GREEN, msg);		}
	__force_inline__ string _YELLOW_(string msg)	{	return COLOR(ANSI_YELLOW, msg);		}
	__force_inline__ string _BLUE_(string msg)		{	return COLOR(ANSI_BLUE, msg);		}
	__force_inline__ string _PURPLE_(string msg)	{	return COLOR(ANSI_PURPLE, msg);		}
	__force_inline__ string _CYAN_(string msg)		{	return COLOR(ANSI_CYAN, msg);		}
	__force_inline__ string _WHITE_(string msg)		{	return COLOR(ANSI_WHITE, msg);		}
	__force_inline__ string _BOLD_(string msg)		{	return COLOR(ANSI_BOLD, msg);		}
	__force_inline__ string _UNDERLINE_(string msg)	{	return COLOR(ANSI_UNDERLINE, msg);	}
	__force_inline__ string _ITALIC_(string msg)	{	return COLOR(ANSI_ITALIC, msg);		}


	/**
	 * Wrapper function for printing logging information to screen
	 */
	void print(const char* color, string type, const char* _file_, int line, const char* fmt, va_list& args) {
		if (print_log_type) {
			printf("%s%s%s", color, type.c_str(), ANSI_RESET);
		}

		if (print_timestamps) {
			auto duration = std::chrono::system_clock::now().time_since_epoch();
			unsigned long int millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
			printf("[%ld]", millis);
		}

		if (print_thread_id) {
			std::stringstream ss; ss << std::this_thread::get_id();
			unsigned long long int id = std::stoull(ss.str());
			printf("[%04lld]", id);
		}

		if (print_log_type || print_timestamps || print_thread_id)
			printf(": ");

		if (print_file) {
			printf("%s:", _file_);
		}

		if (print_line) {
			printf("%d:", line);
		}

		if (print_file || print_line)
			printf(": ");

		vfprintf(stdout, fmt, args);
		printf("\n");
		fflush(stdout);
		va_end(args);
	}

	/**
	 * Variadic argument function for printing information logs to screen.
	 */
	void _info_(const char* _file_, int line, const char* fmt, ...) {
		if (!_enable) return;
		va_list args; va_start(args, fmt);
		print(ANSI_GREEN, "[INFO]", _file_, line, fmt, args);
	}

	/**
	 * Variadic argument function for printing error logs to screen.
	 */
	void _error_(const char* _file_, int line, const char* fmt, ...) {
		if (!_enable) return;
		va_list args; va_start(args, fmt);
		print(ANSI_RED, "[ ERR]", _file_, line, fmt, args);
	}

	/**
	 * Variadic argument function for printing warning logs to screen.
	 */
	void _warning_(const char* _file_, int line, const char* fmt, ...) {
		if (!_enable) return;
		va_list args; va_start(args, fmt);
		print(ANSI_BLUE, "[WARN]", _file_, line, fmt, args);
	}

	/**
	 * The following function are used to get the name of a type from its pointer
	 */
	template<typename T>
	const char* get_type(T* c) {
		if (!_enable) return nullptr;
		auto ptr = std::unique_ptr<char, decltype(& std::free)>{
			abi::__cxa_demangle(typeid(*c).name(), nullptr, nullptr, nullptr),
			std::free
		};
		return string(ptr.get()).c_str();
	}

	/**
	 * The following function is used to get the name of a type from its reference
	 */
	#ifndef __clang__
	template<typename T>
	const char* get_type(T& c) {
		if (!_enable) return nullptr;
		auto ptr = std::unique_ptr<char, decltype(& std::free)>{
			abi::__cxa_demangle(typeid(c).name(), nullptr, nullptr, nullptr),
			std::free
		};
		return string(ptr.get()).c_str();
	}
	#endif

}

#endif /* CPPLOGGER_H_ */
