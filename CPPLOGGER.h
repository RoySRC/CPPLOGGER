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
#include <condition_variable>
#include "print_task_manager.h"

using std::cout;
using std::endl;
using std::string;
using std::mutex;

#define __force_inline__ __attribute__((always_inline)) inline



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


#define logger_black(msg) 		logger::_BLACK_(msg).c_str()
#define logger_red(msg)			logger::_RED_(msg).c_str()
#define logger_green(msg) 		logger::_GREEN_(msg).c_str()
#define logger_yellow(msg) 		logger::_YELLOW_(msg).c_str()
#define logger_blue(msg) 		logger::_BLUE_(msg).c_str()
#define logger_purple(msg) 		logger::_PURPLE_(msg).c_str()
#define logger_cyan(msg) 		logger::_CYAN_(msg).c_str()
#define logger_white(msg) 		logger::_WHITE_(msg).c_str()
#define logger_bold(msg) 		logger::_BOLD_(msg).c_str()
#define logger_underline(msg) 	logger::_UNDERLINE_(msg).c_str()
#define logger_italic(msg) 		logger::_ITALIC_(msg).c_str()

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
	mutex mtx; \
	va_list __args__; \
	FILE* _output_stream_ = stdout; \
	std::condition_variable cv; \
	print_task_manager ptm; \
}

namespace logger {

	/**
	 *  Mutex lock for critical section. here critical section
	 *  refers to the block of code responsible for printing
	 *  logging information to screen.
	 */
	extern mutex mtx;

	/**
	 *
	 */
	extern std::condition_variable cv;

	/**
	 * Setting the following to true will print the timestamps
	 * with the logging information
	 */
	extern bool _print_timestamps_;
	#define logger_print_timestamps(v) logger::_print_timestamps_ = v

	/**
	 * Setting the following to true will print the thread id
	 * with the logging info, i.e. the thread responsible for
	 * printing the current log statement
	 */
	extern bool _print_thread_id_;
	#define logger_print_thread_id(v) logger::_print_thread_id_ = v

	/**
	 * Setting the following to false will not print "[INFO]: ", "[ERROR]: ", "[WARN]: "
	 * before log messages.
	 */
	extern bool _print_log_type_;
	#define logger_print_log_type(v) logger::_print_log_type_ = v

	/**
	 * Setting the following to true will the file name from
	 * where the current log statement was invoked
	 */
	extern bool _print_file_;
	#define logger_print_file(v) logger::_print_file_ = v

	/**
	 * Setting the following to true will the line number from
	 * where the current log statement was invoked
	 */
	extern bool _print_line_;
	#define logger_print_line(v) logger::_print_line_ = v

	/**
	 * setting the following to false will disable global logging.
	 */
	extern bool _enable_;
	#define logger_enable(v) logger::_enable_ = v

	/**
	 * The output stream of the logger.
	 */
	extern FILE* _output_stream_;
	#define logger_output_stream(v) \
			logger::_output_stream_ = v; \
			logger::ptm.set_output_stream(v)

	/**
	 * global va_list to optimize for performance on single threads
	 */
	extern va_list __args__;

	/**
	 *
	 */
	extern print_task_manager ptm;


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
	#define __CPPLOGGER_ST__(color, type) { \
		if (_print_log_type_) { \
			fputs("[" color type ANSI_RESET "]", _output_stream_); \
		} \
		if (_print_timestamps_) { \
			const auto duration = std::chrono::system_clock::now().time_since_epoch(); \
			const unsigned long int millis = \
				std::chrono::duration_cast<std::chrono::milliseconds>(duration).count(); \
			fprintf(_output_stream_, "[%ld]", millis); \
		} \
		if (_print_thread_id_) { \
			std::stringstream ss;\
			ss << std::this_thread::get_id(); \
			const unsigned long long int id = std::stoull(ss.str()); \
			fprintf(_output_stream_, "[%04lld]", id); \
		} \
		if (_print_log_type_ || _print_timestamps_ || _print_thread_id_) { \
			fputs(": ", _output_stream_); \
		} \
		if (_print_file_) { \
			fputs(_file_, _output_stream_); \
			fputs(":", _output_stream_);\
		} \
		if (_print_line_) { \
			fprintf(_output_stream_, "%d:", line); \
		} \
		if (_print_file_ || _print_line_) { \
			fputs(": ", _output_stream_); \
		} \
		vfprintf(_output_stream_, fmt, __args__); \
		fputs(ANSI_RESET "\n", _output_stream_); \
		va_end(__args__); \
	}

	/**
	 * wrapper function containing the boilerplate code for multi-threaded
	 * logging
	 */
	__force_inline__
	void __CPPLOGGER_MT__(const char* fmt, const char* color, const char* type, const char* file,
									const int line, va_list& __args__) {
		DEBUG("Before calling ptm.get()");
		size_t index;
		char* b = ptm.get(index);
		DEBUG("After calling ptm.get()");

		if (logger::_print_log_type_) {
			b += sprintf(b, "[%s%s%s]", color, type, ANSI_RESET);
		}
		if (logger::_print_timestamps_) {
			const auto duration = std::chrono::system_clock::now().time_since_epoch();
			const unsigned long int millis =
					std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
			b += sprintf(b, "[%ld]", millis);
		}
		if (logger::_print_thread_id_) {
			std::stringstream ss;
			ss << std::this_thread::get_id();
			const unsigned long long int id = std::stoull(ss.str());
			b += sprintf(b, "[%04lld]", id);
		}
		if (logger::_print_log_type_ || logger::_print_timestamps_ || logger::_print_thread_id_) {
			b += sprintf(b, ": ");
		}
		if (logger::_print_file_) {
			b += sprintf(b, "%s:", file);
		}
		if (logger::_print_line_) {
			b += sprintf(b, "%d:", line);
		}
		if (logger::_print_file_ || logger::_print_line_) {
			b += sprintf(b, ": ");
		}
		b += vsprintf(b, fmt, __args__);
		sprintf(b, ANSI_RESET "\n");

		ptm.completed(index);
		DEBUG("Done");
	}

	/**
	 * Variadic argument function for printing information logs to screen.
	 */
	#define logger_info(...) logger::_info_(__FILE__, __LINE__, __VA_ARGS__)
	inline void _info_(const char* _file_, const int line, const int cvl, const int avl, const char* fmt, ...) {
		if (_enable_ && cvl >= avl) {
			va_start(__args__, fmt);
			__CPPLOGGER_ST__(ANSI_GREEN, "INFO");
		}
	}

	inline void _info_(const char* _file_, const int line, const char* fmt, ...) {
		if (_enable_) {
			va_start(__args__, fmt);
			__CPPLOGGER_ST__(ANSI_GREEN, "INFO");
		}
	}

	#define logger_info_mt(fmt, ...) logger::_info_mt_(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
	inline void _info_mt_(const char* _file_, const int line, const char* fmt, ...) {
		if (_enable_) {
			va_list __args__;
			va_start(__args__, fmt);
			__CPPLOGGER_MT__(fmt, ANSI_GREEN, "INFO", _file_, line, __args__);
			va_end(__args__);
		}
	}

	/**
	 * Variadic argument function for printing error logs to screen.
	 */
	#define logger_error(...) logger::_error_(__FILE__, __LINE__, __VA_ARGS__)
	inline void _error_(const char* _file_, const int line, const int cvl, const int avl, const char* fmt, ...) {
		if (_enable_ && cvl >= avl) {
			va_start(__args__, fmt);
			__CPPLOGGER_ST__(ANSI_RED, " ERR");
		}
	}

	inline void _error_(const char* _file_, const int line, const char* fmt, ...) {
		if (_enable_) {
			va_start(__args__, fmt);
			__CPPLOGGER_ST__(ANSI_RED, " ERR");
		}
	}

	#define logger_error_mt(fmt, ...) logger::_error_mt_(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
	inline void _error_mt_(const char* _file_, const int line, const char* fmt, ...) {
		if (_enable_) {
			va_list __args__;
			va_start(__args__, fmt);
			__CPPLOGGER_MT__(fmt, ANSI_RED, " ERR", _file_, line, __args__);
			va_end(__args__);
		}
	}

	/**
	 * Variadic argument function for printing warning logs to screen.
	 */
	#define logger_warning(...) logger::_warning_(__FILE__, __LINE__, __VA_ARGS__)
	inline void _warning_(const char* _file_, const int line, const int cvl, const int avl, const char* fmt, ...) {
		if (_enable_ && cvl >= avl) {
			va_start(__args__, fmt);
			__CPPLOGGER_ST__(ANSI_PURPLE, "WARN");
		}
	}

	inline void _warning_(const char* _file_, const int line, const char* fmt, ...) {
		if (_enable_) {
			va_start(__args__, fmt);
			__CPPLOGGER_ST__(ANSI_PURPLE, "WARN");
		}
	}

	#define logger_warning_mt(fmt, ...) logger::_warning_mt_(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
	inline void _warning_mt_(const char* _file_, const int line, const char* fmt, ...) {
		if (_enable_) {
			va_list __args__;
			va_start(__args__, fmt);
			__CPPLOGGER_MT__(fmt, ANSI_BLUE, "WARN", _file_, line, __args__);
			va_end(__args__);
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
