#include <pthread.h>
#include <stdio.h>

#define N 4
#define M 1000000

long long counter = 0;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void *launch(void* args) {
	for (int i = 0; i < M; ++i) {
		if (pthread_mutex_lock(&mut) != 0) {
			perror("failed to lock");
			return NULL;
		}
		counter++;
		if (pthread_mutex_unlock(&mut) != 0) {
                        perror("failed to unlock");
                        return NULL;
                }
	}
}

int main() {
	pthread_t threads[N];
	for (int i = 0; i < N; ++i) {
		if (pthread_create(threads + i, NULL, launch, NULL)) {
			perror("failed to create");
			return 1;
		}
	}

	for (int i = 0; i < N; ++i) {
		if (pthread_join(threads[i], NULL) != 0) {
			perror("failed to join");
			return 1;
		}
	}

	printf("expected: %lld, actual: %lld\n", (long long)M * N, counter);
}
