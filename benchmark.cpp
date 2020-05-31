/*
 * main.cpp
 *
 *  Created on: May 27, 2020
 *      Author: sajeeb
 */

#include <iostream>
#include <thread>
#include <CPPLOGGER.h>

void bench(size_t iters, const char* msg) {
	using std::chrono::duration;
	using std::chrono::duration_cast;
	using std::chrono::high_resolution_clock;

	auto start = high_resolution_clock::now();
	for(size_t i=0; i<iters; ++i) {
		logger::info("Hello world!");
	}

	auto delta = high_resolution_clock::now() - start;
	double delta_d = duration_cast<duration<double>>(delta).count();

	logger::output_stream(stdout);
	logger::enable();
	logger::info("%-16s| Elapsed: %.2f secs | Throughput: %'d/sec", msg, delta_d, int(iters/delta_d));
}

void single_threaded(size_t iters) {
	logger::info("*******************************************************************");
	logger::info("Single threaded benchmark with %'ld iterations", iters);
	logger::info("*******************************************************************");

	logger::output_stream(fopen("/dev/null", "w"));
	bench(iters, "basic");

	logger::output_stream(fopen("/dev/null", "w"));
	logger::disable();
	bench(iters, "disabled");
}

void multi_thread_bench(size_t iters, size_t id, double* data) {
	using std::chrono::duration;
	using std::chrono::duration_cast;
	using std::chrono::high_resolution_clock;

	auto start = high_resolution_clock::now();
	for(size_t i=0; i<iters; ++i) {
		logger::info("Hello world!");
	}

	auto delta = high_resolution_clock::now() - start;
	double delta_d = duration_cast<duration<double>>(delta).count();
	data[id] = delta_d;
}

void multi_threaded(size_t threads, size_t iters) {
	logger::info("*******************************************************************");
	logger::info("%'d thread benchmark with %'ld iterations", threads, iters);
	logger::info("*******************************************************************");

	logger::output_stream(fopen("/dev/null", "w"));
	std::thread _threads[threads];
	double* data = new double [threads];
	for (size_t i=0; i<threads; ++i) _threads[i] = std::thread(multi_thread_bench, iters, i, data);
	for (std::thread& t : _threads) t.join();

	logger::output_stream(stdout);
	logger::enable();
	double delta_d = 0.0;
	for (size_t i=0; i<threads; ++i) delta_d += data[i];
	delta_d /= double(threads);
	logger::info("%-16s| Elapsed: %.3f secs | Throughput: %'d/sec", "basic", delta_d, int(iters/delta_d));

	logger::output_stream(fopen("/dev/null", "w"));
	logger::disable();
	for (size_t i=0; i<threads; ++i) _threads[i] = std::thread(multi_thread_bench, iters, i, data);
	for (std::thread& t : _threads) t.join();

	logger::output_stream(stdout);
	logger::enable();
	delta_d = 0.0;
	for (size_t i=0; i<threads; ++i) delta_d += data[i];
	delta_d /= double(threads);
	logger::info("%-16s| Elapsed: %.3f secs | Throughput: %'d/sec", "disabled",
			delta_d, int(iters/delta_d));

	delete[] data;
}

int main() {
	logger::print_file(false);
	logger::print_line(false);
	logger::print_log_type(false);
	logger::flush_immediately(false);

	setlocale(LC_NUMERIC, "");
	int iters = 1000000;
	size_t threads = 10;

	single_threaded(iters);
	logger::info("");
	multi_threaded(threads, iters);
}
