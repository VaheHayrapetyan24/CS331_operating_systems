#include <stdio.h>
#include <stdlib.h>

int main() {
	size_t n;
	printf("input a positive integer: ");
	scanf("%lu", &n);

	int* int_arr = malloc(n * sizeof(int));

	if (int_arr == NULL) {
		exit(1);
	}

	int sum = 0;
	for (int i = 0, c; i < n; ++i) {
		printf("input an integer: ");
		scanf("%d", int_arr + i);
		sum += int_arr[i];
	}

	printf("sum is %d\n", sum);
	free(int_arr);
}

