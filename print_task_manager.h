/*
 * print_task_manager.h
 *
 *  Created on: Jun 4, 2020
 *      Author: sajeeb
 */

#include <iostream>
#include <atomic>
#include <math.h>
#include <mutex>
#include <threads.h>
#include <assert.h>
#include <condition_variable>
#include <queue>

#ifndef PRINT_TASK_MANAGER_H_
#define PRINT_TASK_MANAGER_H_

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

using std::mutex;
using std::cout;
using std::endl;
using std::thread;
using std::condition_variable;
using std::queue;

#define __force_inline__ __attribute__((always_inline)) inline

class print_task_manager {
private:
	std::atomic<unsigned long long int> idx;
	bool* volatile occupied_slots;
	bool __run__ = true;

	mutex mtx_idx, mtx_completed, mtx_count, mtx_print_thread, mtx_tasks;
	condition_variable cv, ptcv;

	thread printer_thread;
	size_t size = pow(2, 2);

	struct Buffer {
		std::atomic<char**> buffer;
		size_t count = 0;
	};

	Buffer* buff;
	queue<Buffer*> tasks;

	/**
	 *
	 */
	char** allocate() {
		char** buffer = new char* [size];
		for (size_t i=0; i<size; ++i) {
			buffer[i] = new char [1024] ();
		}
		return buffer;
	}

	/**
	 *
	 */
	void deallocate(char** buffer) {
		for (size_t i=0; i<size; ++i) delete[] buffer[i];
		delete[] buffer;
		buffer = NULL;
	}

	/**
	 *
	 */
	inline bool is_full() {
		std::lock_guard<mutex> lock(mtx_count);
		assert(buff->count <= size); // if count is greater than size, there is something wrong
		return (buff->count == size);
	}

	/**
	 *
	 */
	inline void increment_count() {
		std::lock_guard<mutex> lock(mtx_count);
		buff->count += 1;
	}

	/**
	 *
	 */
	inline void add_task(Buffer* buffer) {
		std::lock_guard<mutex> lock(mtx_tasks);
		tasks.push(buff);
		ptcv.notify_all();
	}

	/**
	 *
	 */
	inline void flush() {
		setvbuf(_output_stream_, NULL, _IONBF, 0);
		std::lock_guard<mutex> lock(mtx_tasks);
		while (!tasks.empty()) {
			Buffer* buffer = tasks.front(); tasks.pop();
			for (size_t i=0; i<buffer->count; ++i) {
				fprintf(_output_stream_, "%s", buffer->buffer[i]);
			}
			deallocate(buffer->buffer);
			delete buffer;
		}
	}

	/**
	 *
	 */
	static void run(print_task_manager& ptm) {
		std::unique_lock<mutex> lock(ptm.mtx_print_thread);
		ptm.ptcv.wait(lock,[&](){
			if (!ptm.__run__) return true;
			else {
				ptm.flush();
				return false;
			}
		});
	}


public:
	FILE* _output_stream_ = stdout;

	void set_output_stream(FILE* os) {
		add_task(buff);
		buff = new Buffer();
		buff->buffer = allocate();
		_output_stream_ = os;
	}

	/**
	 *
	 */
	print_task_manager() {
		printer_thread = std::thread(run, std::ref(*this));
		printer_thread.detach();
		occupied_slots = new bool [size] ();
		buff = new Buffer();
		buff->buffer = allocate();
	}

	/**
	 *
	 */
	~print_task_manager() {
		add_task(buff);
		delete[] occupied_slots;
		{
			std::unique_lock<mutex> lock(mtx_print_thread);
			__run__ = false;
		}
		ptcv.notify_all();
	}

	/**
	 *
	 */
	char* get(size_t& i) {
		std::unique_lock<mutex> lck(mtx_completed);
		size_t index = idx++;
		index %= size;
		if (occupied_slots[index] == false) { // the current index is not being used
			occupied_slots[index] = true;

		} else { // the current index is being used
			cv.wait(lck, [&](){
				if (occupied_slots[index] == false) {
					occupied_slots[index] = true;
					return true;
				}
				return false;
			});
		}
		i = index;
		return buff->buffer[i];
	}

	/**
	 *
	 */
	void completed(size_t i) {
		DEBUG("completed()");
		std::lock_guard<mutex> lock(mtx_completed);
		increment_count();
		if (is_full()) {
			DEBUG("_print_thread_ is full, dispatching.");
			add_task(buff);
			buff = new Buffer();
			buff->buffer = allocate();
			memset(occupied_slots, false, size);
			cv.notify_all();
		} else DEBUG("_print_task_ is not full yet");
		DEBUG("completed() Done..");
	}
};



#endif /* PRINT_TASK_MANAGER_H_ */
