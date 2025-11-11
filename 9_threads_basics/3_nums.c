#define _GNU_SOURCE
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

void *launch(void* args) {
	for (int i = 0; i < 5; ++i)
		printf("thread %d, i: %d\n", gettid(), i);
}

int main() {
	pthread_t threads[3];
	for (int i = 0; i < 3; ++i) {
		if (pthread_create(threads + i, NULL, launch, NULL) != 0) {
			perror("failed to create");
			return -1;
		}
	}

	for (int i = 0; i < 3; ++i) {
		if (pthread_join(threads[i], NULL) != 0) {
			perror("failed to join");
			return -1;
		}
	}

	printf("main finished");

}
