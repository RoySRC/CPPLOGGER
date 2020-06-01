/*
 * main.cpp
 *
 *  Created on: May 27, 2020
 *      Author: sajeeb
 */

#include <iostream>
#include <thread>
#include <CPPLOGGER.h>

/**
 * The following should always be called only in the main translation unit.
 * This is to default initialize the logger flags.
 */
logger_init();

FILE* null_file = fopen("/dev/null", "w");

void bench(size_t iters, const char* msg) {
	using std::chrono::duration;
	using std::chrono::duration_cast;
	using std::chrono::high_resolution_clock;

	auto start = high_resolution_clock::now();
	for(size_t i=0; i<iters; ++i) {
		logger::info("Hello logger: msg number %ld", i);
	}

	auto delta = high_resolution_clock::now() - start;
	auto delta_d = duration_cast<duration<double>>(delta).count();

	logger::output_stream(stdout);
	logger::enable(true);
	logger::info("%-16s| Elapsed: %04.2f secs | Throughput: %d/sec", msg, delta_d, int(iters/delta_d));
}

void single_threaded(size_t iters) {
	logger::info("*******************************************************************");
	logger::info("Single threaded benchmark with %ld iterations", iters);
	logger::info("*******************************************************************");

	logger::output_stream(null_file);
	bench(iters, "basic");

	logger::output_stream(null_file);
	logger::enable(false);
	bench(iters, "disabled");
}

void multi_thread_bench(size_t n_threads, size_t iters, string type) {
	using std::chrono::duration;
	using std::chrono::duration_cast;
	using std::chrono::high_resolution_clock;

	std::thread _threads[n_threads];
	for (size_t i=0; i<n_threads; ++i) _threads[i] = std::thread([&]() {
        for (int j = 0; j < int(iters / n_threads); j++) {
			logger::info_mt("Hello logger: msg number %d", j);
        }
    });

	auto start = high_resolution_clock::now();

	for (auto &t : _threads) {
		t.join();
	};

	auto delta = high_resolution_clock::now() - start;
	auto delta_d = duration_cast<duration<double>>(delta).count();

	logger::output_stream(stdout);
	logger::enable(true);
	logger::info("%-16s| Elapsed: %04.2f secs | Throughput: %d/sec", type.c_str(), delta_d, int(iters/delta_d));
}

void multi_threaded(size_t threads, size_t iters) {
	logger::info("*******************************************************************");
	logger::info("%ld thread benchmark with %ld iterations", threads, iters);
	logger::info("*******************************************************************");

	logger::output_stream(null_file);
	multi_thread_bench(threads, iters, "basic");

	logger::output_stream(null_file);
	logger::enable(false);
	multi_thread_bench(threads, iters, "disabled");
}

int main(int argc, char** argv) {
	unsigned int iters = 1000000;
	size_t threads = 10;

	for (uint8_t i=0; i<argc; ++i) {
		if (strcmp(argv[i], "--threads") == 0) {
			threads = atoi(argv[i+1]);
			i += 1;

		} else if (strcmp(argv[i], "--iters") == 0) {
			iters = atoi(argv[i+1]);
			i += 1;
		}
	}

	single_threaded(iters);
	logger::info("");
	multi_threaded(threads, iters);

	fclose(null_file);
}
