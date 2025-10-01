#include <stdio.h>
#include <stdlib.h>

int main() {
	char* arr = malloc(3 * 51 * sizeof(char));
	if (arr == NULL) {
		exit(1);
	}


	for (int i = 0; i < 3 * 51; ++i) {
		printf("input a string: ");
		scanf("%50s", arr + i);
		i += 50;
		arr[i] = '\0';
	}


	for (int i = 0; i < 3 * 51; i += 51) {
		printf("%s\n", arr + i);
	}

	arr = realloc(arr, 5 * 51 * sizeof(char));

	if (arr == NULL) {
		exit(1);
	}

	for (int i = 3 * 51; i < 5 * 51; ++i) {
		printf("input a string: ");
		scanf("%50s", arr + i);
		i += 50;
		arr[i] = '\0';
	}


	for (int i = 0; i < 5 * 51; i += 51) {
		printf("%s\n", arr + i);
	}

	free(arr);
}

