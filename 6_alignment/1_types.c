#include <stdio.h>
#include <stdint.h>

int main() {
	int a;
	char b;
	double c;
	short d;
	
	printf("a: %lu, %p, %lu\n", sizeof(a), &a, (uintptr_t)&a % sizeof(a));
	printf("b: %lu, %p, %lu\n", sizeof(b), &b, (uintptr_t)&b % sizeof(b));
	printf("c: %lu, %p, %lu\n", sizeof(c), &c, (uintptr_t)&c % sizeof(c));
	printf("d: %lu, %p, %lu\n", sizeof(d), &d, (uintptr_t)&d % sizeof(d));

}
