/*
 * thread_pool.h
 *
 *  Created on: Jun 4, 2020
 *      Author: sajeeb
 */
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "print_task.h"

#ifndef PRINT_THREAD_H_
#define PRINT_THREAD_H_

using std::cout;
using std::endl;
using std::thread;
using std::queue;
using std::mutex;
using std::condition_variable;

class printer_thread {
private:
	queue<const char**> tasks;
	mutex mtx, mtx_task;
	condition_variable cv;
	bool __run__;
	thread threads;

	static void run(printer_thread& pt) {
		std::unique_lock<mutex> lck(pt.mtx);
		pt.cv.wait(lck,[&](){
			if (!pt.__run__) return true;
			else {
				pt.flush();
				return false;
			}
		});
	}

public:
	FILE* _output_stream_ = stdout;

	printer_thread() {
		threads = thread(run, std::ref(*this));
		__run__ = true;
		threads.detach();
	}

	~printer_thread() {
		__run__ = false;
		cv.notify_all();
		flush();
	}

	void add_task(const char** t) {
		std::lock_guard<mutex> lock(mtx_task);
		tasks.push(t);
		cv.notify_all();
	}

	void flush() {
		std::lock_guard<mutex> lock(mtx_task);
		while (!tasks.empty()) {
			print_task* t = tasks.front();
			tasks.pop();
			t->flush(_output_stream_);
			delete t;
		}
	}

};


#endif /* PRINT_THREAD_H_ */
