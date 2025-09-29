#include <stdio.h>
#include <stdlib.h>

int main() {
	int* arr = malloc(10 * sizeof(int));
	if (arr == NULL) {
		exit(1);
	}

	for (int i = 0; i < 10; ++i) {
		printf("input an integer: ");
		scanf("%d", arr + i);
	}

	for (int i = 0; i < 10; ++i) {
		printf("%d ", arr[i]);
	}
	printf("\n");

	arr = realloc(arr, 5 * sizeof(int));

	if (arr == NULL) {
		exit(1);
	}

	for (int i = 0; i < 5; ++i) {
		printf("%d ", arr[i]);
	}

	free(arr);
}

