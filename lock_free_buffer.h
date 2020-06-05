/*
 * lock_free_buffer.h
 *
 *  Created on: Jun 3, 2020
 *      Author: sajeeb
 */
#include <mutex>
#include <math.h>
#include <atomic>
#include <condition_variable>
#include <stdio.h>

#define __force_inline__ __attribute__((always_inline)) inline

/**
 * This is a lock free line buffer class encapsulating the line buffer,
 * an atomic unsigned integer object that acts as the index in the buffer,
 * and an unsigned integer representing the size of the buffer. The only
 * guarantee this class provides is that if multiple threads try to access
 * the buffer by index, each of the threads will get a different index in
 * the buffer to access.
 */
class lock_free_buffer {
private:

	/**
	 * delete the current buffer
	 */
	__force_inline__
	void delete_buffer() {
		for (uint i=0; i<size; ++i) delete[] buffer[i];
		delete[] buffer;
		delete[] mutexes;
		buffer = NULL;
		mutexes = NULL;
	}

	/**
	 * Allocate a line buffer of a given size.
	 */
	__force_inline__
	void allocate_buffer() {
		buffer = new char* [size];
		mutexes = new std::mutex [size];
		for (uint i=0; i<size; ++i) {
			buffer[i] = new char [1024];
			buffer[i][0] = '\0';
		}
	}

public:
	std::mutex* mutexes;
	std::atomic_uint idx;
	uint size;
	char** buffer;
	FILE* _output_stream_ = stdout;

	lock_free_buffer() {
		size = pow(2, 1);
		allocate_buffer();
	}

	~lock_free_buffer() {
		flush_buffer();
		delete_buffer();
	}

	/**
	 * Set the new buffer size. First flush the contents of the old buffer to screen.
	 */
	__force_inline__
	void set_buffer_size(const uint size) {
		if (size != this->size) {
			flush_buffer();
			delete_buffer();
			this->size = size;
			allocate_buffer();
		}
	}

	/**
	 * Flush the contents of the buffer to screen.
	 */
	__force_inline__
	void flush_buffer() {
		for (uint i=0; i<size; ++i) {
			fputs(this->buffer[i], _output_stream_);
			buffer[i][0] = '\0';
		}
		idx = 0;
	}

	/**
	 * Get a particular line from the buffer 2D character array.
	 */
	__force_inline__
	char* operator[] (uint index) {
		return buffer[index];
	}
};
