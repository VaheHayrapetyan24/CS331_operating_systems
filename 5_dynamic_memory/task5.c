#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main() {
	size_t n;
	printf("input a positive integer: ");
	scanf("%lu", &n);

	int* int_arr = malloc(n * sizeof(int));

	if (int_arr == NULL) {
		exit(1);
	}

	unsigned int high = 0, low = UINT_MAX;
	for (int i = 0, c; i < n; ++i) {
		printf("input a grade: ");
		scanf("%u", int_arr + i);
		
		if (int_arr[i] < low) low = int_arr[i];
		if (int_arr[i] > high) high = int_arr[i];
	}

	printf("highest %u, lowest %u\n", high, low);
	free(int_arr);
}

