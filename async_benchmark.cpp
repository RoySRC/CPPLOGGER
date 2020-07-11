/*
 * async_benchmark.cpp
 *
 *  Created on: Jun 9, 2020
 *      Author: sajeeb
 */

#include <assert.h>
#include <iostream>
#include <thread>

#include "CPPLOGGER_ASYNC.h"


logger_init();

/**
 * Check when opening files - can an attacker redirect it (via symlinks),
 * force the opening of special file type (e.g., device files), move
 * things around to create a race condition, control its ancestors, or
 * change its contents? (CWE-362)
 */
FILE* null_file = fdopen(open("/dev/null", O_NOFOLLOW|O_RDWR), "w");

void bench(size_t iters, const char* msg) {
	using std::chrono::duration;
	using std::chrono::duration_cast;
	using std::chrono::high_resolution_clock;

	auto start = high_resolution_clock::now();
	for(size_t i=0; i<iters; ++i) {
		logger_async_info("Hello logger: msg number %ld", i);
	}

	auto delta = high_resolution_clock::now() - start;
	auto delta_d = duration_cast<duration<double>>(delta).count();

	logger_output_stream(stdout);
	logger_enable_global(true);
	logger_async_info("%-16s| Elapsed: %04.2f secs | Throughput: %'d/sec", msg, delta_d, int(iters/delta_d));
//	assert(logger::queue._last.load()->stream == stdout);
}

void single_threaded(size_t iters) {
	logger_async_info("*******************************************************************");
	logger_async_info("Single threaded benchmark with %'ld iterations", iters);
	logger_async_info("*******************************************************************");

	logger_output_stream(null_file);
	bench(iters, "basic");

	logger_output_stream(null_file);
	logger_enable_global(false);
	bench(iters, "disabled");
}

void multi_thread_bench(size_t n_threads, size_t iters, string type) {
	using std::chrono::duration;
	using std::chrono::duration_cast;
	using std::chrono::high_resolution_clock;

	std::thread _threads[n_threads];
	for (size_t i=0; i<n_threads; ++i) _threads[i] = std::thread([&]{
        for (int j = 0; j < int(iters / n_threads); j++) {
			logger_async_info("Hello logger: msg number %d", j);
        }
    });

	auto start = high_resolution_clock::now();

	for (auto &t : _threads) {
		t.join();
	};

	auto delta = high_resolution_clock::now() - start;
	auto delta_d = duration_cast<duration<double>>(delta).count();

	logger_output_stream(stdout);
	logger_enable_global(true);
	logger_async_info("%-16s| Elapsed: %04.2f secs | Throughput: %'d/sec", type.c_str(), delta_d, int(iters/delta_d));
}

void multi_threaded(size_t threads, size_t iters) {
	logger_async_info("*******************************************************************");
	logger_async_info("%'d thread benchmark with %'ld iterations", threads, iters);
	logger_async_info("*******************************************************************");

	logger_output_stream(null_file);
	multi_thread_bench(threads, iters, "basic");

	logger_output_stream(null_file);
	logger_enable_global(false);
	multi_thread_bench(threads, iters, "disabled");
}

int main(int argc, char** argv) {
	logger_print_file(false);
	logger_print_line(false);

	setlocale(LC_NUMERIC, "");
	unsigned int iters = 1000000;
	size_t threads = 10;

	for (uint8_t i=0; i<argc; ++i) {
		if (strcmp(argv[i], "--threads") == 0) {
			threads = std::stoul(argv[i+1]);
			if (threads == 0 || threads >= 1000) {
				printf("Number of threads needs to be between (0, 1000)");
				exit(-1);
			}
			i += 1;

		} else if (strcmp(argv[i], "--iters") == 0) {
			iters = std::stoul(argv[i+1]);
			if (iters == 0 || iters >= 10000000) {
				printf("number of messages needs to be within (0, 10000000)");
				exit(-1);
			}
			i += 1;
		}
	}
	single_threaded(iters);
	logger_async_info("");
	multi_threaded(threads, iters);

	fclose(null_file);
}


