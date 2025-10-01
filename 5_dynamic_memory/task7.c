#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>


#define PTR_SIZE sizeof(unsigned int*)

void* aligned_malloc(size_t size, size_t alignment) {
	if (alignment == 0) return NULL;

	size_t _size = size + alignment + sizeof(uintptr_t) - 1;

	void* _ptr = malloc(_size);
	if(_ptr == NULL) return NULL;

	uintptr_t temp_ptr = (uintptr_t)_ptr + sizeof(uintptr_t);
	uintptr_t p_alignment = (uintptr_t) alignment;

	printf("modulo %ju, size %lu ,", temp_ptr % p_alignment, sizeof(uintptr_t));

	temp_ptr += temp_ptr < p_alignment ? p_alignment - temp_ptr : temp_ptr % p_alignment;


	
	printf("original ptr %ju, fake start %ju\n", (uintptr_t)_ptr, temp_ptr);

	*((uintptr_t*)(temp_ptr - sizeof(uintptr_t))) = (uintptr_t)_ptr;


	
	printf("original ptr %ju, start stored in %ju, start value %ju, returned ptr %ju\n", (uintptr_t)_ptr, temp_ptr - sizeof(uintptr_t), *((uintptr_t*)(temp_ptr - sizeof(uintptr_t))), temp_ptr);
	
	return (void*)temp_ptr;
}


void aligned_free(void* ptr) {
	uintptr_t temp_ptr = (uintptr_t)ptr - sizeof(uintptr_t);

	void* _ptr = (void*) (*(uintptr_t*)temp_ptr);

	free(_ptr);
}



int main() {

	int* int_arr = aligned_malloc(156 * sizeof(int), 16);

	if (int_arr == NULL) {
		exit(1);
	}


	aligned_free(int_arr);
}

