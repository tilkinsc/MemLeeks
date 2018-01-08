
#include <stdlib.h>
#include <stdio.h>

#include "leak.h"


typedef struct _ExampleStruct {
	size_t a;
	size_t b;
	void* ptr1;
	void* ptr2;
	int c;
	int d;
} ExampleStruct;


int main(int argc, char** argv)
{
	if(_mem_leak_init() == 0) {
		fputs("Failed to open memleak memleak detection device", stderr);
		exit(EXIT_FAILURE);
	}
	
	ExampleStruct* ex_str = __malloc(sizeof(ExampleStruct), __LINE__);
	ex_str->a = (size_t) 1;
	ex_str->b = (size_t) 2;
	ex_str->c = (int) 3;
	ex_str->d = (int) 4;
	ex_str->ptr1 = __malloc(sizeof(size_t), __LINE__);
	ex_str->ptr2 = __calloc(1, sizeof(size_t), __LINE__);
	
	
	fprintf(stdout, "a %Iu\n", ex_str->a);
	fprintf(stdout, "b %Iu\n", ex_str->b);
	fprintf(stdout, "c %d\n", ex_str->c);
	fprintf(stdout, "d %d\n", ex_str->d);
	fprintf(stdout, "p1 %p\n", ex_str->ptr1);
	fprintf(stdout, "p2 %p\n\n", ex_str->ptr2);
	
	
	size_t size = _gather_size();
	vp_info* ptr_inf = NULL;
	fprintf(stdout, "There are %Iu malloc/calloc handles\n", size);
	for(size_t i=0; i<size; i++) {
		ptr_inf = _gather_get(i);
		fprintf(stdout, "On line %Iu pointer %p exists of type %s length %Iu\n", ptr_inf->line, ptr_inf->ptr, (ptr_inf->ftype == 0 ? "MALLOC" : "CALLOC"), ptr_inf->size);
	}
	
	fprintf(stdout, "\nRealloced %p", ex_str->ptr1);
	ex_str->ptr1 = __realloc(ex_str->ptr1, 10 * sizeof(size_t), __LINE__);
	fprintf(stdout, " to %p\n", ex_str->ptr1);
	
	fputs("\nCleaning environment!\n\n", stdout);
	_free(ex_str);
	
	size = _gather_size();
	fprintf(stdout, "There are %Iu malloc/calloc handles\n", size);
	for(size_t i=0; i<size; i++) {
		ptr_inf = _gather_get(i);
		fprintf(stdout, "On line %Iu pointer %p exists of type %s length %Iu\n", ptr_inf->line, ptr_inf->ptr, (ptr_inf->ftype == 0 ? "MALLOC" : "CALLOC"), ptr_inf->size);
	}
	
	_mem_leak_close();
}
