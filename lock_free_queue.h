/*
 * lock_free_queue.h
 *
 *  Created on: Jun 5, 2020
 *      Author: sajeeb
 */

#ifndef LOCK_FREE_QUEUE_H_
#define LOCK_FREE_QUEUE_H_

#include <chrono>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <atomic>
#include <mutex>
#include <thread>
#include <assert.h>
#include <condition_variable>

//#define __LOGGER_DEBUGGER__
#include "logger_debugger.h"

using std::atomic;
using std::thread;
using std::condition_variable;
using std::mutex;

#define __force_inline__ __attribute__((always_inline)) inline

class lock_free_queue {
public:
	typedef struct node {
		char data[1024] = {'\0', };
		node* next = nullptr;
		bool done = false;
		FILE* stream = stdout;
		node(FILE* os) : stream(os) {}
	} node;

	atomic<node*> _first;
	atomic<node*> _last;
	atomic<int> num_nodes;
	atomic<bool> __kill__;
	atomic<bool> __terminated__;
	uint flush_interval = pow(2, 2);
	atomic<uint> current_flush_count;
	FILE* _output_stream_;
	thread printer;
	condition_variable cv;
	mutex mtx;

	lock_free_queue() {
		num_nodes.store(0);
		current_flush_count.store(0);
		__terminated__.store(false);
		__kill__.store(false);

		node* dummy = new node(_output_stream_);
		_first.store(dummy);
		_last.store(dummy);

		_first.store(nullptr);
		_last.store(nullptr);

		_output_stream_ = (FILE*)stdout;
		printer = thread(run, std::ref(*this));
		printer.detach();
	}

	~lock_free_queue() {
		DEBUG("In Destructor.");
		DEBUG("Terminating print thread");
		__kill__.store(true);
		while (!__terminated__.load()) cv.notify_all();
		DEBUG("Print thread terminated.");
		DEBUG("Number of nodes remaining: %d", num_nodes.load());
		flush_all();
	}

	__force_inline__
	void set_output_stream(FILE* os) {
		_output_stream_ = os;
	}

	__force_inline__
	node* get() {
		node* rv = append();
		cv.notify_one();
		return rv;
	}


private:
	__force_inline__
	node* append() {
		DEBUG("Appending node..");
		node* n = new node(_output_stream_);
		node* prev = _last.exchange(n);
//		prev->next = n;
		DEBUG("Checking previous node pointer.");
		(prev) ? prev->next = n : _first = n;
		DEBUG("Done appending node.");
		return n;
	}

	__force_inline__
	void flush(node* n) {
		_first.store(n->next);
		fprintf(n->stream, "%s", n->data);
		delete n;
	}

	__force_inline__
	void flush_all() {
		DEBUG("flush_all..");
		node* f;
		node* prev_last_node = _last.load();
		while ((f = _first.load()) != nullptr) {
			flush(f);
		}
		_last.compare_exchange_strong(prev_last_node, f);
	}

	__force_inline__
	static void run(lock_free_queue& q) {
		std::unique_lock<std::mutex> lock(q.mtx);
		q.cv.wait_for(lock, std::chrono::milliseconds(1), [&]{
//		q.cv.wait(lock, [&]{
			for (;q._first.load() != q._last.load() && q._first.load()->done;) {
				q.flush(q._first.load());
			}
			return (q.__kill__.load()) ? true : false;
		});
		while (!q.__terminated__.load()) q.__terminated__.store(true);
	}
};

#endif /* LOCK_FREE_QUEUE_H_ */
