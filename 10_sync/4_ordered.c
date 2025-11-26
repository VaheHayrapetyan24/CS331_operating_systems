#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#define N 3
#define M 100
sem_t sems[N];


void *execute(void* args) {
	int j = *(int*)args;

	for (int i = 0; i < M; ++i) {
		sem_wait(sems + j);
		printf("Thread %d: %d\n", j, i);
		sem_post(sems + ((j + 1) % N));
	} 
	printf("finished thread %d\n", j);
}


int main() {

	for (int i = 0; i < N; ++i) {
		if (sem_init(sems + i, 0, i == 0) != 0) { // just initing the 0th one to 1, others will be 0
			perror("failed to init");
			return 0;
		}
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

	for (int i = 0; i < N; ++i) {
                if (sem_destroy(sems + i) != 0) {
                        perror("failed to destroy"); // if one fails,still try the others
                }
        }
}
