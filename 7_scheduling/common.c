#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct {
	int pid;
	int arrival_time;
	int burst_time;
	int turnaround_time;
	int waiting_time;
	int response_time;
} typedef process_t;

process_t arr[10] = {
	{1, 0, 5, 0, 0},
	{2, 0, 2, 0, 0},
	{3, 1, 4, 0, 0},
	{4,14, 8, 0, 0},
	{5, 5, 1, 0, 0},
	{6,17, 2, 0, 0},
	{7,16, 1, 0, 0},
	{8,15, 4, 0, 0},
	{9,22, 3, 0, 0},
       {10,20, 2, 0, 0} 


};

void quicksort(process_t* arr, const unsigned int s, bool (*compare)(process_t*, process_t*)) {
	int* q = malloc(sizeof(int) * s * 2);
	int q_end = 2;
	q[0] = 0;
	q[1] = s;


	int ind, pivot, curr_start, curr_end;
	process_t temp;
	while (q_end) {
		pivot = q[q_end - 2];
		curr_start = pivot;
		ind = pivot + 1;
		curr_end = q[q_end - 1];

		q_end -= 2;

		if (curr_end - curr_start <= 1) {
			continue;
		}
	
		for (;ind < curr_end; ++ind) {
			// compare == true -> swap
			if (compare(arr + pivot, arr + ind)) {
				temp = arr[ind];
				arr[ind] = arr[pivot + 1];
				arr[pivot + 1] = arr[pivot];
				arr[pivot] = temp;

				pivot++;
			}
		}

		// push left pivot up to current pivot
		q[q_end++] = curr_start;
		q[q_end++] = pivot; // the for goes up to end - 1
	
		// push right pivot up to current end
		q[q_end++] = pivot + 1;
		q[q_end++] = curr_end;
	}

	free(q);
}

/*
bool compare(process_t* a, process_t* b) {
	return a->arrival_time > b->arrival_time;
}
*/

void calculate(process_t* arr, int s, float* avg_wait, float* avg_turn) {
	int curr_time = 0;
	for (int i = 0; i < s; ++i) {
		curr_time = curr_time - arr[i].arrival_time < 0 ? arr[i].arrival_time : curr_time;
		arr[i].waiting_time = curr_time - arr[i].arrival_time;
		arr[i].response_time = arr[i].waiting_time;
		arr[i].turnaround_time = arr[i].waiting_time + arr[i].burst_time;

		curr_time += arr[i].burst_time;

		*avg_wait += arr[i].waiting_time;
		*avg_turn += arr[i].turnaround_time;
	}
	*avg_wait /= s;
	*avg_turn /= s;
}


void print_processes(process_t* arr, int s, float* avg_wait, float* avg_turn) {
	
	printf("Gantt Chart:\n");
	for (int i = 0; i < s; ++i) {
		printf("P%*d:%*s|%*s|\n", 5, arr[i].pid, arr[i].waiting_time + arr[i].arrival_time + i, "", arr[i].burst_time, "");
	
	}	

	printf("%*sPID%*sAT%*sBT%*sWT%*sTAT%*sRT\n", 2, "", 3, "", 3, "", 3, "", 2, "", 3, "");
	for (int i = 0; i < s; ++i) {
		printf("%*d%*d%*d%*d%*d%*d\n", 5, arr[i].pid, 5, arr[i].arrival_time, 5, arr[i].burst_time, 5, arr[i].waiting_time, 5, arr[i].turnaround_time, 5, arr[i].response_time); 
	}

}

int run(void(*sort)(process_t*, int)) {

	sort(arr, 10);

	for (int i = 0; i < 10; ++i) {
		printf("%d ", arr[i].pid);
	}
	printf("\n");

	float avg_wait, avg_turn;
	calculate(arr, 10, &avg_wait, &avg_turn);
	print_processes(arr, 10, &avg_wait, &avg_turn);

}

