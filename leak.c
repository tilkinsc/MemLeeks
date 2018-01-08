
#include <stdlib.h> // size_t, memory allocation

#include "leak.h" // memory allocation

#include "darr.h" // array_*


#define INIT_SIZE	10
#define EXPAND_SIZE	10


static Array* arr = 0;


int _mem_leak_init() {
	arr = array_new(INIT_SIZE, EXPAND_SIZE, sizeof(FuncType*));
	if(arr == NULL)
		return 0;
	return 1;
}

void _mem_leak_close() {
	array_free(arr);
}


void* _malloc(size_t size) {
	return __malloc(size, 0);
}

void* __malloc(size_t size, size_t line) {
	vp_info* cont = malloc(sizeof(vp_info));
	cont->ptr = malloc(size);
	cont->size = size;
	cont->line = line;
	cont->ftype = MALLOC;
	array_push(arr, cont);
	return cont->ptr;
}


void* _calloc(size_t nmemb, size_t size) {
	return __calloc(nmemb, size, 0);
}

void* __calloc(size_t nmemb, size_t size, size_t line) {
	vp_info* cont = malloc(1 * sizeof(vp_info));
	cont->ptr = calloc(nmemb, size);
	cont->size = nmemb * size;
	cont->line = line;
	cont->ftype = CALLOC;
	array_push(arr, cont);
	return cont->ptr;
}


void* _realloc(void* ptr, size_t new_size) {
	return __realloc(ptr, new_size, 0);
}

void* __realloc(void* ptr, size_t new_size, size_t line) {
	for(size_t i=0; i<arr->size; i++) {
		vp_info* info = (vp_info*) array_get(arr, i);
		if(info->ptr == ptr) {
			info->ptr = realloc(ptr, new_size);
			info->size = new_size;
			info->line = line;
			return info->ptr;
		}
	}
	return 0;
}


void _free(void* ptr) {
	if(ptr == 0) return;
	free(ptr);
	for(size_t i=0; i<arr->size; i++) {
		vp_info* info = (vp_info*) array_get(arr, i);
		if(info->ptr == ptr) {
			array_rem(arr, i);
			return;
		}
	}
}


size_t _gather_size() {
	return arr->size;
}

vp_info* _gather_get(size_t index) {
	return (vp_info*) array_get(arr, index);
}

