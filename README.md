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
	* For **bold**, __underline__ and *italicizing* text, or a combination of any or all three
	* Provides support for three levels of logging: `[INFO]`, `[ERROR]`, `[WARN]`
	* Flag for enabling or disabling the printing of:
		* Time stamps
		* Threads IDs for logging at thread level for Multi/Single threaded applications
		* Logging types
		* File name and Line numbers
	* Provides a flag to disable global logging, when this flag is set all logging is disabled
	* Enabling/Disabling logging does not require flags to be passed in at compile time
	
	
## Usage examples
### Basic usage
```c++
#include <CPPLOGGER.h>

int main() {

	logger::info("---------------------------------------------------------------------");
	logger::info("Welcome to CPPLOGGER!");
	logger::info("---------------------------------------------------------------------");

	logger::info("The following shows some basic usage.");

	logger::error("This is an error message with some %s:", "argument")
	logger::warning("This is a warning message with 2 args: (%.3f, %.4f)", 1.2323745, 2.5);
	logger::info("This is an info message with three arguments: (%03d, %03d, %03d)", 0, 50, 100);

}
```