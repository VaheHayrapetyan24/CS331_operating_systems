#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#define N 4
#define M 1000000

#define P 3
#define C 5
#define BUFFER_SIZE 8
#define TARGET 200

int buffer[BUFFER_SIZE];
int in_pos = 0;
int out_pos = 0;


int produced = 0;
int consumed = 0;

sem_t empty_slots;
sem_t full_slots;


long long counter = 0;
pthread_mutex_t p_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t c_mut = PTHREAD_MUTEX_INITIALIZER;

void *produce(void* args) {
	int sval;
	while (1) {
		sem_wait(&empty_slots);
		pthread_mutex_lock(&p_mut);
		if (produced != TARGET) {
			buffer[in_pos] = produced;
			in_pos = (in_pos + 1) % BUFFER_SIZE;
			produced++;
			sem_post(&full_slots);
		}
		pthread_mutex_unlock(&p_mut);
		if (produced == TARGET) {
			break;
		}
	}	
	printf("finished produce\n");	
}

void *consume(void* args) {
        while (1) {
		if (sem_trywait(&full_slots) == 0) {
	                pthread_mutex_lock(&c_mut);
			if (consumed != TARGET) {
        	        	printf("consumed %d at %d\n", buffer[out_pos], out_pos);
                		out_pos = (out_pos + 1) % BUFFER_SIZE;
                		consumed++;

				sem_post(&empty_slots);
			}
			pthread_mutex_unlock(&c_mut);
		}
		if (consumed == TARGET) {
			break;
		}
        }
	printf("finished consume\n");
}


int main() {

	if (sem_init(&empty_slots, 0, BUFFER_SIZE) + sem_init(&full_slots, 0, 0) != 0) {
		perror("failed to init");
		return 1;
	}

	pthread_t threads[P + C];
	for (int i = 0; i < P + C; ++i) {
		void* (*fn)(void*) = produce;
		if (i >= P) {
			fn = consume;
		}
		if (pthread_create(threads + i, NULL, fn, NULL)) {
			perror("failed to create");
			return 1;
		}
	}

	for (int i = 0; i < P + C; ++i) {
		if (pthread_join(threads[i], NULL) != 0) {
			perror("failed to join");
			return 1;
		}
	}

	printf("produced: %d, consumed: %d\n", produced, consumed);

	if (sem_destroy(&empty_slots) + sem_destroy(&full_slots) != 0) {
                perror("failed to destroy");
                return 1;
        }
}
