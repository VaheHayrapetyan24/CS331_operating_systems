#define _GNU_SOURCE
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

void *launch(void* arg) {
	int val = *(int*)arg;
	printf("thread %d, square of %d is %d\n", gettid(), val, val * val);
}

int main() {
	pthread_t threads[5];
	int arr[5] = {1,2,3,4,5};
	for (int i = 0; i < 5; ++i) {
		if (pthread_create(threads + i, NULL, launch, arr + i) != 0) {
			perror("failed to create");
			return -1;
		}
	}

	for (int i = 0; i < 5; ++i) {
		if (pthread_join(threads[i], NULL) != 0) {
			perror("failed to join");
			return -1;
		}
	}

	printf("main finished");

}
