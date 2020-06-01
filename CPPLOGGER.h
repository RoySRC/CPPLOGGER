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

#define FMT_HEADER_ONLY
#include "fmt/include/fmt/core.h"
#include "fmt/include/fmt/os.h"
#include "fmt/include/fmt/printf.h"
#include "fmt/include/fmt/ranges.h"

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

/**
 * macro to initialize the extern variables in logger
 */
#define logger_init() \
namespace logger { \
	bool _print_timestamps_ = false; \
	bool _print_thread_id_ = false; \
	bool _print_log_type_ = true; \
	bool _print_file_ = true; \
	bool _print_line_ = true; \
	bool _enable_ = true; \
	bool _flush_immediately_ = false; \
	mutex critical_section; \
	va_list __args__; \
	FILE* _output_stream_ = stdout; \
}

namespace logger {

	/**
	 *  Mutex lock for critical section. here critical section
	 *  refers to the block of code responsible for printing
	 *  logging information to screen.
	 */
	extern mutex critical_section;

	/**
	 * Setting the following to true will print the timestamps
	 * with the logging information
	 */
	extern bool _print_timestamps_;
	#define print_timestamps(v) _print_timestamps_ = v

	/**
	 * Setting the following to true will print the thread id
	 * with the logging info, i.e. the thread responsible for
	 * printing the current log statement
	 */
	extern bool _print_thread_id_;
	#define print_thread_id(v) _print_thread_id_ = v

	/**
	 * Setting the following to false will not print "[INFO]: ", "[ERROR]: ", "[WARN]: "
	 * before log messages.
	 */
	extern bool _print_log_type_;
	#define print_log_type(v) _print_log_type_ = v

	/**
	 * Setting the following to true will the file name from
	 * where the current log statement was invoked
	 */
	extern bool _print_file_;
	#define print_file(v) _print_file_ = v

	/**
	 * Setting the following to true will the line number from
	 * where the current log statement was invoked
	 */
	extern bool _print_line_;
	#define print_line(v) _print_line_ = v

	/**
	 * setting the following to false will disable global logging.
	 */
	extern bool _enable_;
	#define enable(v) _enable_ = v

	/**
	 * When the following is set to true all logging is immediately flushed to
	 * output stream. If false, the logs are stored in a buffer maintained by
	 * the OS and are flushed to the output stream when this buffer is full.
	 */
	extern bool _flush_immediately_;
	#define flush_immediately(v) _flush_immediately_ = v

	/**
	 * The output stream of the logger.
	 */
	extern FILE* _output_stream_;
	#define output_stream(v) _output_stream_ = v

	/**
	 * global va_list to optimize for performance on single threads
	 */
	extern va_list __args__;

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
	 * The following set of macros take in a string object as argument and return a constant char pointer
	 */
	#define COLOR(color, msg) color+msg;
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
	 * Wrapper macro boilerplate code for printing logging information to screen
	 */
	#define print(color, type) { \
		if (_print_log_type_) { \
			fputs("[" color type ANSI_RESET "]", _output_stream_); \
		} \
		if (_print_timestamps_) { \
			const auto duration = std::chrono::system_clock::now().time_since_epoch(); \
			const unsigned long int millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count(); \
			fmt::fprintf(_output_stream_, "[%ld]", millis); \
		} \
		if (_print_thread_id_) { \
			std::stringstream ss; ss << std::this_thread::get_id(); \
			const unsigned long long int id = std::stoull(ss.str()); \
			fmt::fprintf(_output_stream_, "[%04lld]", id); \
		} \
		if (_print_log_type_ || _print_timestamps_ || _print_thread_id_) { \
			fputs(": ", _output_stream_); \
		} \
		if (_print_file_) { \
			fputs(_file_, _output_stream_); \
			fputs(":", _output_stream_);\
		} \
		if (_print_line_) { \
			fmt::fprintf(_output_stream_, "%d:", line); \
		} \
		if (_print_file_ || _print_line_) { \
			fputs(": ", _output_stream_); \
		} \
		vfprintf(_output_stream_, format, __args__); \
		fputs(ANSI_RESET "\n", logger::_output_stream_); \
		va_end(__args__);\
	}

	/**
	 * wrapper function containing the boilerplate code for multi-threaded
	 * logging
	 */
	#define __mt__(format, color, type) \
				va_list __args__; \
				va_start(__args__, format); \
				critical_section.lock(); \
				print(color, type); \
				critical_section.unlock()

	/**
	 * Variadic argument function for printing information logs to screen.
	 */
	#define info(format, ...) _info_(__FILE__, __LINE__, format, ##__VA_ARGS__)
	inline void _info_(const char* _file_, const int line, const char* format, ...) {
		if (_enable_) {
			va_start(__args__, format);
			print(ANSI_GREEN, "INFO");
		}
	}

	#define info_mt(format, ...) _info_mt_(__FILE__, __LINE__, format, ##__VA_ARGS__)
	inline void _info_mt_(const char* _file_, const int line, const char* format, ...) {
		if (_enable_) {
			__mt__(format, ANSI_GREEN, "INFO");
		}
	}

	/**
	 * Variadic argument function for printing error logs to screen.
	 */
	#define error(format, ...) _error_(__FILE__, __LINE__, format, ##__VA_ARGS__)
	inline void _error_(const char* _file_, const int line, const char* format, ...) {
		if (_enable_) {
			va_start(__args__, format);
			print(ANSI_RED, " ERR");
		}
	}

	#define error_mt(format, ...) _error_mt_(__FILE__, __LINE__, format, ##__VA_ARGS__)
	inline void _error_mt_(const char* _file_, const int line, const char* format, ...) {
		if (_enable_) {
			__mt__(format, ANSI_RED, " ERR");
		}
	}

	/**
	 * Variadic argument function for printing warning logs to screen.
	 */
	#define warning(format, ...) _warning_(__FILE__, __LINE__, format, ##__VA_ARGS__)
	inline void _warning_(const char* _file_, const int line, const char* format, ...) {
		if (_enable_) {
			va_start(__args__, format);
			print(ANSI_BLUE, "WARN");
		}
	}

	#define warning_mt(format, ...) _warning_mt_(__FILE__, __LINE__, format, ##__VA_ARGS__)
	inline void _warning_mt_(const char* _file_, const int line, const char* format, ...) {
		if (_enable_) {
			__mt__(format, ANSI_BLUE, "WARN");
		}
	}

	/**
	 * The following function are used to get the name of a type from its pointer
	 */
	template<typename T>
	const char* get_type(T* c) {
		if (!_enable_) return nullptr;
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
		if (!_enable_) return nullptr;
		auto ptr = std::unique_ptr<char, decltype(& std::free)>{
			abi::__cxa_demangle(typeid(c).name(), nullptr, nullptr, nullptr),
			std::free
		};
		return string(ptr.get()).c_str();
	}
	#endif

}

#endif /* CPPLOGGER_H_ */
