# CPPLOGGER

This is a simple, lightweight and thread safe logger for C++. 


## Platforms
* Linux
* MacOS (`get_type` function might not work with clang compilers)
	
	
## Features
* Header only, no need for separate compilation and linking
* ANSI unicode support for:
	* For printing logging information in eight colors
		* Black
		* Red
		* Green
		* Blue
		* Yellow
		* Cyan
		* Purple/ Magenta
		* White
	* For **bold**, underline and *italicizing* text, or a combination of any or all three
	* Provides support for three levels of logging: `[INFO]`, `[ERROR]`, `[WARN]`
	* Flag for enabling or disabling the printing of:
		* Time stamps
		* Threads IDs for logging at thread level for Multi/Single threaded applications
		* Logging types
		* File name and Line numbers
* Provides a flag to disable global logging, when this flag is set all logging is disabled
* Enabling/Disabling logging does not require flags to be passed in at compile time
* Provides a flag to enable or disable immediate logging. When immediate logging is enabled, the `printf` buffer gets immediately flushed to the output stream. When immediate logging is disabled, the print buffer only gets flushed when it is full. When this buffer gets flushed is also determined by the OS. Disabling immediate logging provides a significant performance boost as outlined in the [Benchmarks](#Benchmarks) section. As a result of the performance gains, immediate logging is disabled by default.
* Provides verbose level option to limit how much logging information gets printed. Refer to [Verbose Level](#### Verbose Level) example.
	
	
## Usage examples
#### Basic usage
```c++
#include <CPPLOGGER.h>

/**
 * The following should always be called only in the main translation unit.
 * This is to default initialize the logger flags.
 */
logger_init();

int main() {

	logger::info("-------------------------------------------------------------");
	logger::info("Welcome to CPPLOGGER!");
	logger::info("-------------------------------------------------------------");

	logger::info("The following shows some basic usage.");

	logger::error("This is an error message with some %s:", "argument")
	logger::warning("This is a warning message with 2 args: (%.3f, %.4f)", 1.2323745, 2.5);
	logger::info("This is an info message with three arguments: (%03d, %03d, %03d)", 0, 50, 100);

}
```

#### Typographic examples with colors
```c++
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
```

#### Disabling global logging example
```c++
#include <CPPLOGGER.h>

/**
 * The following should always be called only in the main translation unit.
 * This is to default initialize the logger flags.
 */
logger_init();

int main() {

	logger::info("This is before disabling global logging.");

	logger::enable(false);
	logger::info("This should not be printed.");

	logger::enable(true);
	logger::info("This is after enabling global logging.");

}
```

#### Printing logging time stamps
```c++
#include <CPPLOGGER.h>

/**
 * The following should always be called only in the main translation unit.
 * This is to default initialize the logger flags.
 */
logger_init();

int main() {

	logger::info("This is printed without a time stamp.");

	logger::print_timestamps = true;
	logger::info("This is printed with a time stamp.");

}
```

#### Logging with multiple threads
```c++
#include <CPPLOGGER.h>

/**
 * The following should always be called only in the main translation unit.
 * This is to default initialize the logger flags.
 */
logger_init();

void print_thread_id(uint id) {
	logger::info_mt("%s: %03d", logger::RED("Message from thread"), id);
	logger::info_mt("Message from thread: %03d", id);
}

int main() {
	const uint n_threads = 200;
	std::thread threads[n_threads];
	for (uint8_t i=0; i<n_threads; ++i)
		threads[i] = std::thread(print_thread_id, i+1);
	for (std::thread& t : threads) t.join();
}
```

####  Logging with printing thread IDs
```c++
#include <CPPLOGGER.h>

/**
 * The following should always be called only in the main translation unit.
 * This is to default initialize the logger flags.
 */
logger_init();

void print_thread_id(uint id) {
	logger::print_thread_id(true);
	logger::info_mt("%s: %03d", logger::RED("Message from thread"), id);
	logger::info_mt("Message from thread: %03d", id);
}

int main() {
	const uint n_threads = 200;
	std::thread threads[n_threads];
	for (uint8_t i=0; i<n_threads; ++i)
		threads[i] = std::thread(print_thread_id, i+1);
	for (std::thread& t : threads) t.join();
}
```

#### Logging without printing the log type
```c++
#include <CPPLOGGER.h>

/**
 * The following should always be called only in the main translation unit.
 * This is to default initialize the logger flags.
 */
logger_init();

int main() {

	logger::info("This message is printed with log type enabled.");

	logger::print_log_type = false;
	logger::info("This message is printed without the log type.");

}
```

#### Logging without printing the file name
```c++
#include <CPPLOGGER.h>

/**
 * The following should always be called only in the main translation unit.
 * This is to default initialize the logger flags.
 */
logger_init();

int main() {

	logger::info("This message is printed with the filename.");

	logger::print_file = false;
	logger::info("This message is printed without printing the filename.");

}
```

#### Logging without printing the line number
```c++
#include <CPPLOGGER.h>

/**
 * The following should always be called only in the main translation unit.
 * This is to default initialize the logger flags.
 */
logger_init();

int main() {

	logger::info("This message is printed with the line number.");

	logger::print_line = false;
	logger::info("This message is printed without the line number.");

}
```

#### Logging without printing either the filename or the line number
```c++
#include <CPPLOGGER.h>

/**
 * The following should always be called only in the main translation unit.
 * This is to default initialize the logger flags.
 */
logger_init();

int main() {

	logger::info("This message has both line number and file");

	logger::print_file = false;
	logger::print_line = false;
	logger::info("This message does not have either a file or a line number.");

}
```

#### Verbose Level 
```c++
#include <CPPLOGGER.h>

/**
 * The following should always be called only in the main translation unit.
 * This is to default initialize the logger flags.
 */
logger_init();

int main() {
	int verbose_level = 0;

	logger::info(verbose_level, 1, "This should only be printed at verbose level 1.");

	logger::info("Current verbose level: %d", verbose_level);
	logger::info("Incrementing verbose level by one: %d", ++verbose_level);
	logger::info("Current verbose level: %d", verbose_level);

	logger::info(verbose_level, 1, "This should only be printed at verbose level 1.");
}

```

#### Get type of a variable
```c++
#include <CPPLOGGER.h>

/**
 * The following should always be called only in the main translation unit.
 * This is to default initialize the logger flags.
 */
logger_init();

class sample_class {
public:
	sample_class() {}
	void print() {
		logger::info("print method in class: %s", logger::RED(logger::get_type(this)));
	}
};

void print_thread_id(uint id) {
	logger::info("%s: %03d", logger::RED("Message from thread"), id);
	logger::info("Message from thread: %03d", id);
}

int main(int argc, char** argv) {

	sample_class c;
	c.print();
	logger::info("c: %s", logger::get_type(c));


	int a = 10;
	logger::info("type of a: %s", logger::get_type(a));
}
```

## Benchmarks
Benchmarking done on Ubuntu 20.04 64 bit, Intel® Core™ i5-6600K CPU @ 4.60GHz × 4 

Throughput is the number of messages printed per second. Iterations is the number of log messages.

#### Benchmark result with immediate flushing disabled
```
[INFO]: *******************************************************************
[INFO]: Single threaded benchmark with 5,000,000 iterations
[INFO]: *******************************************************************
[INFO]: basic           | Elapsed: 0.46 secs | Throughput: 10,955,614/sec
[INFO]: disabled        | Elapsed: 0.01 secs | Throughput: 417,115,448/sec
[INFO]: 
[INFO]: *******************************************************************
[INFO]: 10 thread benchmark with 5,000,000 iterations
[INFO]: *******************************************************************
[INFO]: basic           | Elapsed: 2.45 secs | Throughput: 2,044,349/sec
[INFO]: disabled        | Elapsed: 0.01 secs | Throughput: 453,749,783/sec
```
#### Benchmark result with immediate flushing enabled
```
[INFO]: *******************************************************************
[INFO]: Single threaded benchmark with 5,000,000 iterations
[INFO]: *******************************************************************
[INFO]: basic           | Elapsed: 2.37 secs | Throughput: 2,112,810/sec
[INFO]: disabled        | Elapsed: 0.01 secs | Throughput: 415,817,359/sec
[INFO]: 
[INFO]: *******************************************************************
[INFO]: 10 thread benchmark with 5,000,000 iterations
[INFO]: *******************************************************************
[INFO]: basic           | Elapsed: 4.81 secs | Throughput: 1,038,932/sec
[INFO]: disabled        | Elapsed: 0.01 secs | Throughput: 425,526,881/sec
```
