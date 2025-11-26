#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define N 10 // ++
#define M 10 // --

int c = 10000000;

long long balance = 0;

int speed = 1; // 0 = slow, 1 = fast
int lock_type = 0; // 0 = mutex, 1 = spinlock
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_spinlock_t spin;



void lock() {
	if (lock_type) {
		if (pthread_spin_lock(&spin) != 0) {
			perror("failed to lock");
			exit(1);
		}
		return;
	}

	if (pthread_mutex_lock(&mut) != 0) {
		perror("failed to lock");
		exit(1);
	}
}

void unlock() {
        if (lock_type) {
                if (pthread_spin_unlock(&spin) != 0) {
                        perror("failed to lock");
                        exit(1);
                }
                return;
        }

        if (pthread_mutex_unlock(&mut) != 0) {
                perror("failed to lock");
                exit(1);
        }
}

void *deposit(void* args) {
	for (int i = 0; i < c; ++i) {
		lock();
		if (!speed) {
			usleep(100);
		}
		balance++;
		unlock();
	}
}


void *withdraw(void* args) {
	for (int i = 0; i < c; ++i) {
		lock();
		if (!speed) {
			usleep(100);
		}
		balance--;
		unlock();
	}
}


int main(int argc, char* argv[]) {
	if (argc > 1 && strcmp(argv[1], "spinlock") == 0) { // default is mutex, this is the spinlock case
		lock_type = 1;
		if (pthread_spin_init(&spin, PTHREAD_PROCESS_PRIVATE) != 0) {
			perror("failed to init spinlock");
			return 1; 
		}
	}

	if (argc > 2 && strcmp(argv[2], "slow") == 0) { // default is fast
		speed = 0;
		c = 1000;
	}

	clock_t start_time, end_time;
	start_time = clock();

	pthread_t threads[N + M];
	void*(*fn)(void*) = deposit;
	for (int i = 0; i < N + M; ++i) {
		if (i == N) {
			fn = withdraw;
		}

		if (pthread_create(threads + i, NULL, fn, NULL)) {
			perror("failed to create");
			return 1;
		}
	}

	for (int i = 0; i < N + M; ++i) {
		if (pthread_join(threads[i], NULL) != 0) {
			perror("failed to join");
			return 1;
		}
	}
	end_time = clock();
	double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
	printf("execution time: %f seconds\n", time_taken);

	printf("actual: %lld\n", balance);
}
