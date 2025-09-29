#include <stdio.h>
#include <stdlib.h>

int main() {
	size_t n;
	printf("input a positive integer: ");
	scanf("%lu", &n);

	int* int_arr = calloc(n, sizeof(int));

	if (int_arr == NULL) {
		exit(1);
	}

	printf("new array:");
	for (int i = 0; i < n; ++i) {
		printf("%d ", int_arr[i]);
	}
	printf("\n");

	int sum = 0;
	for (int i = 0; i < n; ++i) {
		printf("input an integer: ");
		scanf("%d", int_arr + i);
		sum += int_arr[i];
	}

	printf("updated array: ");
	for (int i = 0; i < n; ++i) {
		printf("%d ", int_arr[i]);
	}
	printf("\n");

	printf("avg is %f\n", (float)sum / n);
	free(int_arr);
}

