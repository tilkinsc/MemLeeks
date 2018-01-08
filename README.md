# MemLeeks
Very simplex, cross-platform methodology for detecting memory leaks in C programs  

There is probably better out there. This should be laggy as heck in a real, big program. Although, for testing certain variables, this does justice.

Did you know? Using `__LINE__` in your code is a macro that expands to the line number your code is on?

<details><summary>Project Test Output</summary><p>
  
```
C:\git\MemLeeks>test
a 1
b 2
c 3
d 4
p1 00000000002DF380
p2 00000000002DF3D0

There are 3 malloc/calloc handles
On line 25 pointer 00000000002DF320 exists of type MALLOC length 40
On line 30 pointer 00000000002DF380 exists of type MALLOC length 8
On line 31 pointer 00000000002DF3D0 exists of type CALLOC length 8

Realloced 00000000002DF380 to 00000000005776C0

Cleaning environment!

There are 2 malloc/calloc handles
On line 51 pointer 00000000005776C0 exists of type MALLOC length 80
On line 31 pointer 00000000002DF3D0 exists of type CALLOC length 8
```
</p></details>

# About
```
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
```

**int** _\_mem\_leak\_init_(); - initalizes the inner dynamic array to record memory allocations
**void** _\_mem\_leak\_close_(); - deallocates the inner dynamic array

**void\*** _\_malloc_(_size\_t_ size); - appends a struct to a dynamic array, returns pointer to allocated memory  
**void\*** _\_\_malloc_(_size\_t_ size, _size\_t_ line); - ^ and records line number 

**void\*** _\_calloc_(_size\_t_ nmemb, _size\_t_ size); - appends a struct to a dynamic array, returns pointer to allocated memory  
**void\*** _\_\_calloc_(_size\_t_ nmemb, _size\_t_ size, _size\_t_ line); - ^ and records line number

**void\*** _\_realloc_(_void\*_ ptr, _size\_t_ new\_size); - reallocs a pointer, updates struct, returns pointer to allocated memory    
**void\*** _\_\_realloc_(_void\*_ ptr, _size\_t_ new\_size, _size\_t_ line); - ^ and records line number  

**void** _\_free_(_void\*_ ptr); - frees memory allocated and removes the record of dynamic memory allocation from the array

used to iterate through \*allocs  
1. at end of program, take size  
2. at end of cleanup, take size  
3. if size1 != 0, you **may** have leakage  
4. if size2 != 0, you have leakage  
5. iterate through using \_gather\_get() the leaks  
6. fix the leaks  

**size\_t** _\_gather\_size_(); - returns the number of elements in the dynamic array (for iteration)  
**vp_info\*** _\_gather\_get_(_size\_t_ index); - returns the element at array index index in the dynamic array  

![Leeky :^)](http://s3.amazonaws.com/finecooking.s3.tauntonclud.com/app/uploads/2017/04/24171514/ING-leeks-thumb1x1.jpg)
