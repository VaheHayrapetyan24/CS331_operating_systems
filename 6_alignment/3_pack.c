#include <stdio.h>

#pragma pack
struct {
	char a;
	int b;
	double c;
} typedef struct_a;

#pragma pack(1)
struct {
	char a;
	int b;
	double c;
} typedef struct_b;


#pragma pack(2)
struct {
	char a;
	int b;
	double c;
} typedef struct_c;

#pragma pack(4)
struct {
	char a;
	int b;
	double c;
} typedef struct_d;
#pragma pack(8)
struct {
	char a;
	int b;
	double c;
} typedef struct_e;

int main() {

	struct_a a;
	struct_b b;
	struct_c c;
	struct_d d;
	struct_e e;

	printf("a: %lu %p, b: %lu %p, c: %lu %p, d: %lu %p, %lu %p\n", sizeof(a), &a, sizeof(b), &b, sizeof(c), &c, sizeof(d), &d, sizeof(e), &e);
}
