#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#define N 20
#define K 3

sem_t sem;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;


void *execute(void* args) {
	int j = *(int*)args;
	int val;
	sem_wait(&sem);
	sem_getvalue(&sem, &val);
	printf("Thread %d is printing. Count %d\n", j, val);
	sem_post(&sem);
}


int main() {

	if (sem_init(&sem, 0, K) != 0) {
		perror("failed to init");
		return 0;
	}

	pthread_t threads[N];
	int vals[N];
	for (int i = 0; i < N; ++i) {
		vals[i] = i;
		if (pthread_create(threads + i, NULL, execute, vals + i)) {
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

        if (sem_destroy(&sem) != 0) {
		perror("failed to destroy");
        }
}
