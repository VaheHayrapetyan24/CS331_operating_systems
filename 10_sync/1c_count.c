#include <pthread.h>
#include <stdio.h>

#define N 4
#define M 1000000

long long counter = 0;
pthread_spinlock_t spin;

void *launch(void* args) {
	for (int i = 0; i < M; ++i) {
		if (pthread_spin_lock(&spin) != 0) {
			perror("failed to lock");
			return NULL;
		}
		counter++;
		if (pthread_spin_unlock(&spin) != 0) {
                        perror("failed to unlock");
                        return NULL;
                }
	}
}

int main() {
	if (pthread_spin_init(&spin, PTHREAD_PROCESS_PRIVATE) != 0) {
		perror("failed to init spinlock");
		return 1; 
	}
	pthread_t threads[N];
	for (int i = 0; i < N; ++i) {
		if (pthread_create(threads + i, NULL, launch, NULL)) {
			perror("failed to create");
			pthread_spin_destroy(&spin);
			return 1;
		}
	}

	for (int i = 0; i < N; ++i) {
		if (pthread_join(threads[i], NULL) != 0) {
			perror("failed to join");
			pthread_spin_destroy(&spin);
			return 1;
		}
	}

	pthread_spin_destroy(&spin);
	printf("expected: %lld, actual: %lld\n", (long long)M * N, counter);
}
