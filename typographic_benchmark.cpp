/*
 * typographic_benchmark.cpp
 *
 *  Created on: Jul 12, 2020
 *      Author: sajeeb
 */


#include <CPPLOGGER_SYNC.h>

logger_init();

class Timer {

	using std::chrono::duration;
	using std::chrono::duration_cast;
	using std::chrono::high_resolution_clock;

	high_resolution_clock::time_point start;

public:

	explicit Timer() {
		start = high_resolution_clock::now();
	}

	double elapsed() {
		auto end = high_resolution_clock::now();
		auto delta = end - start;
		auto delta_d = duration_cast<duration<double>>(delta).count();
		return delta_d;
	}
};

void bench() {
	logger_info(logger_bold("-------------------------------------------------------------"));
	logger_info(logger_bold("Welcome to CPPLOGGER!"));
	logger_info(logger_bold("-------------------------------------------------------------"));

	logger_info(logger_red("This is a red text"));
	logger_info(
			logger_red(
					logger_italic("This is a red italic text.")
			)
	);
	logger_info(logger_bold(logger_red("This is a red bold text")));
	logger_info(logger_red(logger_bold(logger_italic("This is a red bold italic text"))));


	logger_info("%s: %d",
			logger_bold(logger_red("This is a red bold text with argument")),
			12);

	logger_info("%s: %s",
			logger_bold(logger_red("This is a red bold text with green argument")),
			logger_green("12"));

	logger_info("%s: %s",
			logger_bold(logger_red("This is a red bold text with green bold argument")),
			logger_bold(logger_green("12")));

	logger_info(logger_underline("%s: %s"),
			logger_bold(logger_red("This is a red bold text with green bold argument")),
			logger_bold(logger_green("12")));
}

int main() {
	FILE* os = fopen("/dev/null", "w");
	logger_output_stream(os);
	uint iters = 1000000;
	{
		Timer timer;
		bench();
		double delta_d = timer.elapsed();
		logger_output_stream(stdout);
		logger_info("%-16s| Elapsed: %04.2f secs | Throughput: %'d/sec",
				"Typographic", delta_d, int(iters/delta_d));
	}
}
