#include <stdio.h>
#include "common.c"

bool compare(process_t* a, process_t* b) {
	return a->arrival_time > b->arrival_time;
}

void sort(process_t* arr, int s) {
	quicksort(arr, s, compare);
}

int main() {
	run(sort);
}

