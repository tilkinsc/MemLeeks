
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef enum _FuncType {
	MALLOC = 0,
	CALLOC = 1
} FuncType;

typedef struct _vp_info {
	void* ptr;
	size_t size;
	size_t line;
	FuncType ftype;
} vp_info;


int _mem_leak_init();
void _mem_leak_close();


void* _malloc(size_t size);
void* __malloc(size_t size, size_t line);

void* _calloc(size_t nmemb, size_t size);
void* __calloc(size_t nmemb, size_t size, size_t line);

void _free(void* ptr);


size_t _gather_size();
vp_info* _gather_get(size_t index);
