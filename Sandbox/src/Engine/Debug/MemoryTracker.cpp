#include "xpch.h"
#include "MemoryTracker.h"
#include "Engine/Debug/Logger.h"
#if defined(DEBUG) || defined(RELEASE)

static size_t allocatedMem = 0;
static size_t freedMem = 0;

static size_t trackMemAlloc = 0;
static size_t trackMemFree = 0;

//returns the amount of memory (in bytes) still allocated from the new operator
size_t get_current_memory_allocated()
{
	return allocatedMem - freedMem;
}

//starts a 'track'
void start_track()
{
	trackMemAlloc = allocatedMem;
	trackMemFree = freedMem;
}

//returns the amount of memory (in bytes) still allocated by the 'new' operator
//since the 'track' started
size_t end_track()
{
	return (allocatedMem - freedMem) - (trackMemAlloc - trackMemFree);
}

//returns the total amount of memory (in bytes) allocated
//throughout the program from the 'new' operator
size_t get_total_memory_allocated()
{
	return allocatedMem;
}

//returns the total amount of memory (in bytes) freed
//throughout the program from the 'new' operator
size_t get_total_memory_freed()
{
	return freedMem;
}


void* operator new(size_t size) {
	allocatedMem += size;
	void* mem = malloc(size);
	return mem;
}

void operator delete(void* mem, size_t size) {
	if (mem) {
		freedMem += size;
		free(mem);
	}
}

void* operator new[](size_t size) {
	void* mem = malloc(size + 1 * sizeof(size_t));
	if (!mem)
		Log_EngineError("Heap Memory Allocation Failed - {0} bytes",size);
	allocatedMem += size + 1 * sizeof(size_t);
	*((size_t*)mem) = size;

	return (void*)&((size_t*)mem)[1];
}

void operator delete[](void* mem) {
	if (mem) {
		freedMem += ((((size_t*)mem)[-1]) + 1 * sizeof(size_t));
		free(&((size_t*)(mem))[-1]);
	}
}

#endif