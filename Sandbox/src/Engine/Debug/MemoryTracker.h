#pragma once

#if defined(DEBUG) || defined(RELEASE)

//returns the amount of memory (in bytes) still allocated on the heap from the 'new' operator
size_t get_current_memory_allocated();

//marks where a 'track' starts
void start_track();

//returns the amount of memory (in bytes) created in the track still allocated
size_t end_track();

//returns the total amount of memory (in bytes) allocated in the program from the 'new' operator
size_t get_total_memory_allocated();

//returns the total amount of memory (in bytes) freed in the program from the 'delete' operator
size_t get_total_memory_freed();

void* operator new(size_t size);

void operator delete(void* mem, size_t size);

void* operator new[](size_t size);
void operator delete[](void* mem);

#define START_MEM_TRACK() start_track()
#define END_MEM_TRACK() printf("%zu \n",end_track());
#define GET_MEM_ALLOCATED() printf("%zu\n",get_current_memory_allocated());

#else

#define START_MEM_TRACK()
#define END_MEM_TRACK()
#define GET_MEM_ALLOCATED()

#endif
