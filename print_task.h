/*
 * task.h
 *
 *  Created on: Jun 4, 2020
 *      Author: sajeeb
 */

#include <iostream>
#include <math.h>
#include <mutex>
#include <assert.h>

#define DEBUG(fmt, ...) /*{\
	const char*  ___file___ = __FILE__;\
	const int ___line___ = __LINE__;\
	std::stringstream ss;\
	ss << std::this_thread::get_id();\
	const unsigned long long int id = std::stoull(ss.str());\
	const auto duration = std::chrono::system_clock::now().time_since_epoch();\
			const unsigned long int millis =\
					std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();\
	printf("[%lld][%ld]: %s:%d:: " fmt ".\n", id, millis, ___file___, ___line___, ##__VA_ARGS__);\
	fflush((FILE*)stdout);\
}*/


#ifndef PRINT_TASK_H_
#define PRINT_TASK_H_

using std::mutex;
using std::cout;
using std::endl;

class print_task {
private:
	void allocate() {
		buffer = new char* [size];
		for (size_t i=0; i<size; ++i) {
			buffer[i] = new char [1024] ();
		}
	}

	void deallocate() {
		for (size_t i=0; i<size; ++i) delete[] buffer[i];
		delete[] buffer;
		buffer = NULL;
	}

public:
	mutex mtx_loc, mtx_flush;
	size_t loc = 0;
	size_t size;
	char** buffer = NULL;

	print_task(size_t size) {
		this->size = size;
		allocate();
	}

	~print_task() {
		deallocate();
	}

	void flush(FILE* os) {
		std::lock_guard<mutex> lock(mtx_flush);
		setvbuf(os, NULL, _IONBF, 0);
		for (size_t i=0; i<size; ++i) {
			fprintf(os, "%s", buffer[i]);
		}
	}

	inline bool is_full() {
		std::lock_guard<mutex> lock(mtx_loc);
		assert(loc <= size);
		return (loc == size);
	}

	inline void increment_count() {
		std::lock_guard<mutex> lock(mtx_loc);
		++loc;
	}
};


#endif /* PRINT_TASK_H_ */
