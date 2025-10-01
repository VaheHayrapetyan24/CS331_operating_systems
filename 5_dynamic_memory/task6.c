#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

void* my_realloc(void* ptr, size_t size) {
	if (size == 0) {
		free(ptr);
		return NULL;
	}


	size_t orig_size = malloc_usable_size(ptr);
	size_t cp_size = orig_size < size ? orig_size : size;

	void* new_ptr = malloc(size);
	if (new_ptr == NULL) {
		return NULL;
	}

	memcpy(new_ptr, ptr, cp_size);

	free(ptr);
	return new_ptr;
}

int main() {
	size_t n = 10;

	int* int_arr = malloc(n * sizeof(int));

	if (int_arr == NULL) {
		exit(1);
	}


	for (int i = 0, c; i < n; ++i) {
		int_arr[i] = i; 
	}


	printf("current usable size %lu\n", malloc_usable_size(int_arr));

	n = 40;
	int_arr = my_realloc(int_arr, n * sizeof(int));

	printf("current usable size %lu\n", malloc_usable_size(int_arr));

	for (int i = 10, c; i < n; ++i) {
		int_arr[i] = i; 
	}	

	for (int i = 0, c; i < n; ++i) {
		printf("%d ", int_arr[i]);
	}
	printf("\n");	

	n = 5;
	int_arr = my_realloc(int_arr, n * sizeof(int));
	printf("current usable size %lu\n", malloc_usable_size(int_arr));

	for (int i = 0, c; i < n; ++i) {
		printf("%d ", int_arr[i]);
	}

	free(int_arr);
}

