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
		char data[256];
		node* next = nullptr;
		bool done = false;
		FILE* stream;
		node(FILE* os) : stream(os) {}
	} node;

	atomic<node*> _first;
	atomic<node*> _last;
	atomic<int> num_nodes;
	atomic<bool> __kill__;
	atomic<bool> __terminated__;
	uint flush_interval = pow(2, 13);
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

		_first.store(NULL);
		_last.store(NULL);

		set_output_stream(stdout);
		printer = thread(lock_free_queue::run, std::ref(*this));
		printer.detach();
	}

	~lock_free_queue() {
		DEBUG("In Destructor.");
		DEBUG("Terminating print thread");
		__kill__.store(true);
		while (!__terminated__.load()) cv.notify_one();
		DEBUG("Print thread terminated.");
		DEBUG("Number of nodes remaining: %d", num_nodes.load());
		flush_all();
	}

	__force_inline__
	node* get() {
		return append();
	}

	__force_inline__
	void set_output_stream(FILE* os) {
//		DEBUG("Value before setting stream: %p", _output_stream_.load());
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
		node* n = new node(_output_stream_);
		++num_nodes;
//		DEBUG("new node: %p", n);
		node* prev = _last.exchange(n);
//		DEBUG("Checking previous node pointer: %p", prev);
		(prev) ? prev->next = n : _first = n;
//		DEBUG("new first: %p", _first.load());
//		DEBUG("Done appending node.");
//		if (++current_flush_count % flush_interval) cv.notify_one();
		cv.notify_one();
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
//		DEBUG("Printed: %s to %p", n->data, n->stream);
//		DEBUG("DELETE: %p", n);
		delete n;
		--num_nodes;
	}

	/**
	 * Gets called only in the destructor
	 */
	__force_inline__
	void flush_all() {
//		DEBUG("flush_all..");
		node* f;
		node* prev_last_node = _last;
		while ((f = _first) != nullptr) {
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
//		q.cv.wait_for(lock, std::chrono::seconds(1), [&]{
		q.cv.wait(lock, [&]{
//			if (q.num_nodes < 8192) return false;
			node* f = q._first;
//			DEBUG("f: %p, l: %p", f, l);
			for (;f != q._last && f != NULL && f->done;) {
//				DEBUG("Printer thread flushing node: %p", f);
				fprintf(f->stream, "%s", f->data);
				const node* n = f;
				f = f->next;
				delete n;
				--q.num_nodes;
			}
			q._first = f;
//			DEBUG("num_nodes: %d", q.num_nodes.load());
//			DEBUG("Printer thread done flushing");
			return (q.__kill__.load()) ? true : false;
		});
		while (!q.__terminated__.load()) q.__terminated__.store(true);
	}
};

#endif /* LOCK_FREE_QUEUE_H_ */
