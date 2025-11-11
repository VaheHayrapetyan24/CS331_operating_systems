#define _GNU_SOURCE
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

void *launch(void* args) {
	int val = *(int*)args;
	printf("thread %d, arg: %d\n", gettid(), val);
}

int main() {
	pthread_t threads[3];
	int ids[3] = {0,1,2};
	for (int i = 0; i < 3; ++i) {
		if (pthread_create(threads + i, NULL, launch, (void*) (ids + i)) != 0) {
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
