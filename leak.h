
#pragma once

#include <stdlib.h> // size_t

// enum tells about type of call made
typedef enum _FuncType {
	MALLOC = 0,
	CALLOC = 1
} FuncType;

// struct to uniformly hold data
typedef struct _vp_info {
	void* ptr; // the pointer allocated
	size_t size; // size of the pointer
	size_t line; // the line, if using _malloc or _calloc, it is always 0
	FuncType ftype; // malloc is 0, calloc is 1
} vp_info;


// initializers and deinitializers
int _mem_leak_init();
void _mem_leak_close();


// mallocs
void* _malloc(size_t size);
void* __malloc(size_t size, size_t line);

// callocs
void* _calloc(size_t nmemb, size_t size);
void* __calloc(size_t nmemb, size_t size, size_t line);

// reallocs
void* _realloc(void* ptr, size_t new_size);
void* __realloc(void* ptr, size_t new_size, size_t line);

// free
void _free(void* ptr);


// used to iterate through *allocs
// 1. at end of program, take size
// 2. at end of cleanup, take size
// 3. compare the sizes, if size1 + size2 != 0, you have leakage
// 4. iterate through using _gather_get() the leaks
// 5. fix the leaks
size_t _gather_size();
vp_info* _gather_get(size_t index);
