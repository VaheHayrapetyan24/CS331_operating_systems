#define _GNU_SOURCE
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

struct {
	int* start;
	int len;
} typedef start_end;

void *launch(void* args) {
	start_end* s = (start_end*)args;

	int sum = 0;
	for (int i = 0; i < s->len; ++i) {
		sum += *(s->start + i);
	}
	printf("thread %d, sum: %d\n", gettid(), sum);
}

int main() {
	pthread_t threads[2];
	start_end args[2];
	int arr[10];
	for (int i = 0; i < 10; ++i) { arr[i] = i; }

	for (int i = 0; i < 2; ++i) {
		args[i].start = arr + i * 5;
		args[i].len = 5;
		if (pthread_create(threads + i, NULL, launch, (void*) (args + i)) != 0) {
			perror("failed to create");
			return -1;
		}
	}

	for (int i = 0; i < 2; ++i) {
		if (pthread_join(threads[i], NULL) != 0) {
			perror("failed to join");
			return -1;
		}
	}

	printf("main finished");

}
