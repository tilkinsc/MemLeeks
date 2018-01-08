
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "leak.h"

#include "darr.h"


#define INIT_SIZE	4
#define EXPAND_SIZE	4


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


void* __malloc(size_t size, size_t line) {
	vp_info* cont = malloc(sizeof(vp_info));
	cont->ptr = malloc(size);
	cont->size = size;
	cont->line = line;
	cont->ftype = MALLOC;
	array_push(arr, cont);
	return cont->ptr;
}

void* _malloc(size_t size) {
	vp_info* cont = malloc(1 * sizeof(vp_info));
	cont->ptr = malloc(size);
	cont->size = size;
	cont->line = 0;
	cont->ftype = MALLOC;
	array_push(arr, cont);
	return cont->ptr;
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

void* _calloc(size_t nmemb, size_t size) {
	vp_info* cont = malloc(1 * sizeof(vp_info));
	cont->ptr = calloc(nmemb, size);
	cont->size = nmemb * size;
	cont->line = 0;
	cont->ftype = CALLOC;
	array_push(arr, cont);
	return cont->ptr;
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

