/*
 * progress.cpp
 *
 *  Created on: Jul 3, 2020
 *      Author: sajeeb
 */


#include <CPPLOGGER_SYNC.h>
#include <unistd.h>

logger_init();

void print_progress_bar(size_t current, size_t total, char bar='=', int width=80) {
	// generate the bars array
	char bars[width+1];
	memset(bars, bar, width);
	bars[width] = '\0';

	const double ratio = current/ (double)total;
	// round the number of bars to the nearest integer
	const uint num_bars = ratio * width;
	const double percentage = ratio * 100.0;

	// print number of bars with window width padding to the right
	printf("[%-*.*s] %3.2f%%\r", width, num_bars, bars, percentage);
	fflush(stdout);
}

int main() {
	size_t current_progress = 0;
	size_t total_progress = 100;
	int window_width = 55; //columns

	for (; current_progress <= total_progress; current_progress += 10) {
		print_progress_bar(current_progress, total_progress, '=', window_width);
		sleep(1);
	}
	// percentage width including '%' sign: 7
	// 1 space
	// 2 for: '[]'
	printf("%-*.s\r", window_width+1+7+2, "");
}
