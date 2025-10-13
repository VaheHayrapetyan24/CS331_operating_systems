#include <stdio.h>

struct {
	char a;
	int b;
	double c;
} typedef struct_a;

struct {
	char a;
	double c;
	int b;
} typedef struct_b;

int main() {

	struct_a a;
	struct_b b;

	printf("a: %lu %p, b: %lu %p\n", sizeof(a), &a, sizeof(b), &b);
}
