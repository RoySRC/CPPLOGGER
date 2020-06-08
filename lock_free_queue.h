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
	uint flush_interval = pow(2, 5);
	atomic<uint> current_flush_count;
	atomic<FILE*> _output_stream_;
	thread printer;
	condition_variable cv;
	mutex mtx;

	lock_free_queue() {
		num_nodes.store(0);
		current_flush_count.store(0);
		__terminated__.store(false);
		__kill__.store(false);

		_first.store(nullptr);
		_last.store(nullptr);

		_output_stream_.store((FILE*)stdout);
		setbuf(_output_stream_.load(), NULL);
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
	node* get() {
		node* rv = append();
//		if (!(current_flush_count++ % flush_interval)) cv.notify_all();
//		cv.notify_all();
		return rv;
	}

	__force_inline__
	void set_output_stream(FILE* os) {
//		DEBUG("Value before setting stream: %p", _output_stream_.load());
//		DEBUG)
		_output_stream_.store(os);
//		setbuf(_output_stream_.load(), NULL);
//		DEBUG("Value after setting stream: %p", _output_stream_.load());
	}


private:
	/**
	 * Only gets called in the get function
	 */
	__force_inline__
	node* append() {
//		DEBUG("Appending node..");
		node* n = new node(_output_stream_.load());
//		DEBUG("new node: %p", n);
		node* prev = _last.exchange(n);
//		DEBUG("Checking previous node pointer: %p", prev);
		(prev) ? prev->next = n : _first = n;
//		DEBUG("new first: %p", _first.load());
//		DEBUG("Done appending node.");
		return n;
	}

	/**
	 * Gets called in flush and run method
	 */
	__force_inline__
	void flush(node* n) {
//		DEBUG("flush");
//		DEBUG("Setting contents of _first to: %p", n->next);
		_first.store(n->next);
//		DEBUG("Printing contents of node: %p", n);
//		setbuf(n->stream, NULL);
		fprintf(n->stream, "%s", n->data);
		DEBUG("Printed: %s to %p", n->data, n->stream);
//		DEBUG("DELETE: %p", n);
		delete n;
	}

	/**
	 * Gets called only in the destructor
	 */
	__force_inline__
	void flush_all() {
//		DEBUG("flush_all..");
		node* f;
		node* prev_last_node = _last.load();
		while ((f = _first.load()) != nullptr) {
			flush(f);
		}
		_last.compare_exchange_strong(prev_last_node, f);
	}

	/**
	 *
	 */
	__force_inline__
	static void run(lock_free_queue& q) {
		std::unique_lock<std::mutex> lock(q.mtx);
//		q.cv.wait_for(lock, std::chrono::microseconds(10), [&]{
		q.cv.wait(lock, [&]{
			node* f = q._first.load();
			node* l = q._last.load();
//			DEBUG("f: %p, l: %p", f, l);
			for (;f != l && f != nullptr && f->done; f = q._first.load()) {
//				DEBUG("Printer thread flushing node: %p", f);
				q.flush(f);
			}
//			DEBUG("Printer thread done flushing");
			return (q.__kill__.load()) ? true : false;
		});
		while (!q.__terminated__.load()) q.__terminated__.store(true);
	}
};

#endif /* LOCK_FREE_QUEUE_H_ */
