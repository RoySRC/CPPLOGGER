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
#include "lock_free_queue.h"

using std::string;
using std::mutex;

#define __force_inline__ __attribute__((always_inline)) inline


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
	bool _flush_immediately_ = false; \
	mutex critical_section; \
	va_list __args__; \
	FILE* _output_stream_ = stdout; \
	lock_free_queue queue;\
	timestamp_resolution _resolution = logger::timestamp_resolution::millisecond;\
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
	 *
	 */
	extern lock_free_queue queue;

	/**
	 * The output stream of the logger.
	 */
	extern FILE* _output_stream_;
	#define logger_output_stream(v) \
			logger::_output_stream_ = v;\
			logger::queue.set_output_stream((FILE*)v)

	/**
	 *
	 */
	enum timestamp_resolution{
		nanosecond, microsecond, millisecond, seconds, minutes, hours
	};
	extern timestamp_resolution _resolution;
	#define logger_nanosecond 	logger::timestamp_resolution::nanosecond
	#define logger_microsecond 	logger::timestamp_resolution::microsecond
	#define logger_millisecond 	logger::timestamp_resolution::millisecond
	#define logger_seconds 		logger::timestamp_resolution::seconds
	#define logger_minutes 		logger::timestamp_resolution::minutes
	#define logger_hours 		logger::timestamp_resolution::hours
	#define logger_days 		logger::timestamp_resolution::days
	#define logger_weeks 		logger::timestamp_resolution::weeks
	#define logger_months 		logger::timestamp_resolution::months
	#define logger_years 		logger::timestamp_resolution::years
	#define logger_timestamp_resolution(v) logger::_resolution = v

	/**
	 *
	 */
	__force_inline__
	void __print_timestamp_printf__() {
		const auto duration = std::chrono::system_clock::now().time_since_epoch();
		switch (_resolution) {
			case nanosecond:
				fprintf(_output_stream_, "[%ld]",
						std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count());
				break;

			case microsecond:
				fprintf(_output_stream_, "[%ld]",
						std::chrono::duration_cast<std::chrono::microseconds>(duration).count());
				break;

			case millisecond:
				fprintf(_output_stream_, "[%ld]",
						std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
				break;

			case seconds:
				fprintf(_output_stream_, "[%ld]",
						std::chrono::duration_cast<std::chrono::seconds>(duration).count());
				break;

			case minutes:
				fprintf(_output_stream_, "[%ld]",
						std::chrono::duration_cast<std::chrono::minutes>(duration).count());
				break;

			case hours:
				fprintf(_output_stream_, "[%ld]",
						std::chrono::duration_cast<std::chrono::hours>(duration).count());
				break;
		}
	}

	/**
	 *
	 */
	__force_inline__
	int __print_timestamp_sprintf__(char* b) {
		int rv = 0;
		const auto duration = std::chrono::system_clock::now().time_since_epoch();
		switch (_resolution) {
			case nanosecond:
				rv += sprintf(b, "[%ld]", std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count());
				break;

			case microsecond:
				rv += sprintf(b, "[%ld]", std::chrono::duration_cast<std::chrono::microseconds>(duration).count());
				break;

			case millisecond:
				rv += sprintf(b, "[%ld]", std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
				break;

			case seconds:
				rv += sprintf(b, "[%ld]", std::chrono::duration_cast<std::chrono::seconds>(duration).count());
				break;

			case minutes:
				rv += sprintf(b, "[%ld]", std::chrono::duration_cast<std::chrono::minutes>(duration).count());
				break;

			case hours:
				rv += sprintf(b, "[%ld]", std::chrono::duration_cast<std::chrono::hours>(duration).count());
				break;
		}
		return rv;
	}

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
	#define __CPPLOGGER_COLOR__(color, msg) color+msg;
	__force_inline__ string _BLACK_(string msg)		{return __CPPLOGGER_COLOR__(ANSI_BLACK, msg);		}
	__force_inline__ string _RED_(string msg)		{return __CPPLOGGER_COLOR__(ANSI_RED, msg);			}
	__force_inline__ string _GREEN_(string msg)		{return __CPPLOGGER_COLOR__(ANSI_GREEN, msg);		}
	__force_inline__ string _YELLOW_(string msg)	{return __CPPLOGGER_COLOR__(ANSI_YELLOW, msg);		}
	__force_inline__ string _BLUE_(string msg)		{return __CPPLOGGER_COLOR__(ANSI_BLUE, msg);		}
	__force_inline__ string _PURPLE_(string msg)	{return __CPPLOGGER_COLOR__(ANSI_PURPLE, msg);		}
	__force_inline__ string _CYAN_(string msg)		{return __CPPLOGGER_COLOR__(ANSI_CYAN, msg);		}
	__force_inline__ string _WHITE_(string msg)		{return __CPPLOGGER_COLOR__(ANSI_WHITE, msg);		}
	__force_inline__ string _BOLD_(string msg)		{return __CPPLOGGER_COLOR__(ANSI_BOLD, msg);		}
	__force_inline__ string _UNDERLINE_(string msg)	{return __CPPLOGGER_COLOR__(ANSI_UNDERLINE, msg);	}
	__force_inline__ string _ITALIC_(string msg)	{return __CPPLOGGER_COLOR__(ANSI_ITALIC, msg);		}


	/**
	 * Wrapper macro boilerplate code for printing logging information to screen
	 */
	#define __CPPLOGGER_PRINT__(color, type) { \
		if (_print_log_type_) { \
			fputs("[" color type ANSI_RESET "]", _output_stream_); \
		} \
		if (_print_timestamps_) { \
			__print_timestamp_printf__(); \
		} \
		if (_print_thread_id_) { \
			std::stringstream ss; ss << std::this_thread::get_id(); \
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
	 *
	 */
	#define __CPPLOGGER_MT__(color, type) {\
		va_list __args__;\
		va_start(__args__, fmt);\
		std::lock_guard<std::mutex> lock(critical_section);\
		__CPPLOGGER_PRINT__(color, type);\
	}

	/**
	 * wrapper function containing the boilerplate code for multi-threaded
	 * logging
	 */
	#define __CPPLOGGER_ASYNC__(fmt, color, type) {\
		va_list __args__; \
		va_start(__args__, fmt); \
		lock_free_queue::node* _node = queue.get();\
		char* b = _node->data;\
		if (_print_log_type_) { \
			b += sprintf(b, "[" color type ANSI_RESET "]"); \
		} \
		if (_print_timestamps_) { \
			b += __print_timestamp_sprintf__(b);\
		} \
		if (_print_thread_id_) { \
			std::stringstream ss; ss << std::this_thread::get_id(); \
			const unsigned long long int id = std::stoull(ss.str()); \
			b += sprintf(b, "[%04lld]", id); \
		} \
		if (_print_log_type_ || _print_timestamps_ || _print_thread_id_) { \
			b += sprintf(b, ": "); \
		} \
		if (_print_file_) { \
			b += sprintf(b, "%s:", _file_); \
		} \
		if (_print_line_) { \
			b += sprintf(b, "%d:", line); \
		} \
		if (_print_file_ || _print_line_) { \
			b += sprintf(b, ": "); \
		} \
		b += vsprintf(b, fmt, __args__); \
		b += sprintf(b, ANSI_RESET "\n"); \
		va_end(__args__);\
		_node->done = true;\
	}

	/**
	 * Variadic argument function for printing information logs to screen.
	 */
	#define logger_info(...) logger::_info_(__FILE__, __LINE__, __VA_ARGS__)
	#define logger_async_info(...) logger::_info_async_(__FILE__, __LINE__, __VA_ARGS__)
	#define logger_info_mt(...) logger::_info_mt_(__FILE__, __LINE__, __VA_ARGS__)

	inline void _info_(const char* _file_, const int line, const int cvl, const int avl, const char* fmt, ...) {
		if (_enable_ && cvl >= avl) {
			va_start(__args__, fmt);
			__CPPLOGGER_PRINT__(ANSI_GREEN, "INFO");
		}
	}

	inline void _info_(const char* _file_, const int line, const char* fmt, ...) {
		if (_enable_) {
			va_start(__args__, fmt);
			__CPPLOGGER_PRINT__(ANSI_GREEN, "INFO");
		}
	}


	inline void _info_mt_(const char* _file_, const int line, const int cvl, const int avl, const char* fmt, ...) {
		if (_enable_ && cvl >= avl) {
			__CPPLOGGER_MT__(ANSI_GREEN, "INFO");
		}
	}

	inline void _info_mt_(const char* _file_, const int line, const char* fmt, ...) {
		if (_enable_) {
			__CPPLOGGER_MT__(ANSI_GREEN, "INFO");
		}
	}

	inline void _info_async_(const char* _file_, const int line, const int cvl, const int avl, const char* fmt, ...) {
		if (_enable_ && cvl >= avl) {
			__CPPLOGGER_ASYNC__(fmt, ANSI_GREEN, "INFO");
		}
	}

	inline void _info_async_(const char* _file_, const int line, const char* fmt, ...) {
		if (_enable_) {
			__CPPLOGGER_ASYNC__(fmt, ANSI_GREEN, "INFO");
		}
	}

	/**
	 * Variadic argument function for printing error logs to screen.
	 */
	#define logger_error(...) logger::_error_(__FILE__, __LINE__, __VA_ARGS__)
	#define logger_async_error(...) logger::_error_async_(__FILE__, __LINE__, __VA_ARGS__)
	#define logger_error_mt(...) logger::_error_mt_(__FILE__, __LINE__, __VA_ARGS__)

	inline void _error_(const char* _file_, const int line, const int cvl, const int avl, const char* fmt, ...) {
		if (_enable_ && cvl >= avl) {
			va_start(__args__, fmt);
			__CPPLOGGER_PRINT__(ANSI_RED, " ERR");
		}
	}

	inline void _error_(const char* _file_, const int line, const char* fmt, ...) {
		if (_enable_) {
			va_start(__args__, fmt);
			__CPPLOGGER_PRINT__(ANSI_RED, " ERR");
		}
	}

	inline void _error_mt_(const char* _file_, const int line, const int cvl, const int avl, const char* fmt, ...) {
		if (_enable_ && cvl >= avl) {
			__CPPLOGGER_MT__(ANSI_RED, " ERR");
		}
	}

	inline void _error_mt_(const char* _file_, const int line, const char* fmt, ...) {
		if (_enable_) {
			__CPPLOGGER_MT__(ANSI_RED, " ERR");
		}
	}

	inline void _error_async_(const char* _file_, const int line, const int cvl, const int avl, const char* fmt, ...) {
		if (_enable_ && cvl >= avl) {
			__CPPLOGGER_ASYNC__(fmt, ANSI_RED, " ERR");
		}
	}

	inline void _error_async_(const char* _file_, const int line, const char* fmt, ...) {
		if (_enable_) {
			__CPPLOGGER_ASYNC__(fmt, ANSI_RED, " ERR");
		}
	}

	/**
	 * Variadic argument function for printing warning logs to screen.
	 */
	#define logger_warning(...) logger::_warning_(__FILE__, __LINE__, __VA_ARGS__)
	#define logger_async_warning(...) logger::_warning_async_(__FILE__, __LINE__, __VA_ARGS__)
	#define logger_warning_mt(...) logger::_warning_mt_(__FILE__, __LINE__, __VA_ARGS__)

	inline void _warning_(const char* _file_, const int line, const int cvl, const int avl, const char* fmt, ...) {
		if (_enable_ && cvl >= avl) {
			va_start(__args__, fmt);
			__CPPLOGGER_PRINT__(ANSI_PURPLE, "WARN");
		}
	}

	inline void _warning_(const char* _file_, const int line, const char* fmt, ...) {
		if (_enable_) {
			va_start(__args__, fmt);
			__CPPLOGGER_PRINT__(ANSI_PURPLE, "WARN");
		}
	}


	inline void _warning_mt_(const char* _file_, const int line, const int cvl, const int avl, const char* fmt, ...) {
		if (_enable_ && cvl >= avl) {
			__CPPLOGGER_MT__(ANSI_BLUE, "WARN");
		}
	}

	inline void _warning_mt_(const char* _file_, const int line, const char* fmt, ...) {
		if (_enable_) {
			__CPPLOGGER_MT__(ANSI_BLUE, "WARN");
		}
	}

	inline void _warning_async_(const char* _file_, const int line, const int cvl, const int avl, const char* fmt, ...) {
		if (_enable_ && cvl >= avl) {
			__CPPLOGGER_ASYNC__(fmt, ANSI_BLUE, "WARN");
		}
	}

	inline void _warning_async_(const char* _file_, const int line, const char* fmt, ...) {
		if (_enable_) {
			__CPPLOGGER_ASYNC__(fmt, ANSI_BLUE, "WARN");
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
