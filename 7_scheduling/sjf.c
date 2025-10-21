#include <stdio.h>
#include "common.c"

void insert_sort(process_t* arr, int s) {
	int cand = 0;

	int time = 0;
	process_t temp;
	for (int i = 0; i < s; ++i) {
		cand = i;
		for (int j = i + 1; j < s; ++j) {
			// cand = j;
			if (arr[j].arrival_time > time) {
				continue;
			}

			if (arr[j].burst_time < arr[cand].burst_time) {
				cand = j;
				continue;
			} 

			if (
				arr[j].burst_time == arr[cand].burst_time && 
				arr[j].arrival_time > arr[cand].arrival_time
			) {
				cand = j;
				continue;
			}
		}

		if (cand != i) {
			temp = arr[cand];
			arr[cand] = arr[i];
			arr[i] = temp;
		}

		time += (time > arr[i].arrival_time ? 0 : arr[i].arrival_time - time) + arr[i].burst_time;
	}

}

int main() {
	run(insert_sort);
}

