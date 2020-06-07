/*
 * lock_free_queue.h
 *
 *  Created on: Jun 5, 2020
 *      Author: sajeeb
 */

#ifndef LOCK_FREE_QUEUE_H_
#define LOCK_FREE_QUEUE_H_

#include <stdlib.h>
#include <iostream>
#include <atomic>
#include <mutex>
#include <threads.h>
#include <assert.h>
#include <condition_variable>

//#define __LOGGER_DEBUGGER__
#include "logger_debugger.h"

#define __force_inline__ __attribute__((always_inline)) inline

using std::atomic;
using std::thread;
using std::condition_variable;
using std::mutex;

class lock_free_queue {
public:
	typedef struct node {
		char data[1024] = {'\0', };
		node* next = nullptr;
		bool done = false;
	} node;

	atomic<node*> _first;
	atomic<node*> _last;
	atomic<int> num_nodes;
	atomic<bool> __kill__;
	atomic<bool> __terminated__;
	uint flush_interval = 4;
	atomic<unsigned long long> current_flush_count;
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
		DEBUG("Setting the output stream.");
		flush_all();
		DEBUG("Finished flushing all nodes.");
		_output_stream_.store(os);
	}

	__force_inline__
	node* get() {
		node* rv = append();
		if (current_flush_count++ % flush_interval == 0) cv.notify_all();
		return rv;
	}


private:
	__force_inline__
	node* append() {
		DEBUG("Appending node..");
		node* n = new node;
//		++num_nodes;
//		DEBUG("Number of nodes: %d", num_nodes.load());
		node* prev = _last.exchange(n);
		DEBUG("Checking previous node pointer.");
		if (prev == nullptr) {
			_first.store(n);
			DEBUG("Previous node pointer is null.");
		} else {
			DEBUG("Previous node pointer is not null.");
			prev->next = n;
		}
		DEBUG("Done appending node.");
		return n;
	}

	__force_inline__
	void flush(node* n) {
		DEBUG("flush..");
		fprintf(_output_stream_.load(), "%s", n->data);
		_first.store(n->next);
		DEBUG("Deleted node: %p", n);
		delete n;
//		--num_nodes;
//		DEBUG("Number of nodes: %d", num_nodes.load());
//		assert(num_nodes >= 0);
	}

	__force_inline__
	void flush_all() {
		DEBUG("flush_all..");
		mtx.lock();
		node* f;
		node* prev_last_node = _last.load();
		while ((f = _first.load()) != nullptr) {
			if (f->done) {
				flush(f);
			} else {
				DEBUG("Waiting for node to finish..");
			}
		}
		_last.compare_exchange_strong(prev_last_node, _first.load());
		mtx.unlock();
	}

	__force_inline__
	static void run(lock_free_queue& q) {
		std::unique_lock<std::mutex> lock(q.mtx);
		q.cv.wait(lock, [&]{
			setbuf(q._output_stream_, NULL);
			node* f = q._first.load();
			DEBUG("Printer thread awake.");
			DEBUG("%p | %p", f, q._last.load());
			for (;f != nullptr && f->done; f = q._first.load()) {
				q.flush(f);
			}
			if (q.__kill__.load()) return true;
			else return false;
		});
		while (!q.__terminated__.load()) q.__terminated__.store(true);
	}
};

#endif /* LOCK_FREE_QUEUE_H_ */
