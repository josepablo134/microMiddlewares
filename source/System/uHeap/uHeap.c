#include <stdint.h>
#include "uHeap.h"

#define HEAP_STRUCT_SIZE	sizeof(struct uheap)
#define MIN_SIZE			HEAP_STRUCT_SIZE
	
struct uheap* uheap_init( uint8_t* heap_buffer , uint16_t heap_size ){
	if( !heap_buffer ){return 0x00;}
	if( heap_size < MIN_SIZE ){ return 0x00; }
	struct uheap*	heap;
	///	Using part of the heap buffer as heap structure
	heap = (struct uheap*) heap_buffer;
	
	///	Setting rest of heap as new heap buffer.
	heap->heap_bytes = (heap_buffer + HEAP_STRUCT_SIZE);
	heap->free = heap_size - HEAP_STRUCT_SIZE;
	heap->length = heap->free;
	return heap;
}

uint8_t*	uheap_malloc( struct uheap* heap_t,  uint16_t size ){
	if(!heap_t){return 0x00;}
	if(!size ){return 0x00;}
	if( heap_t->free < size ){return 0x00;}

	///	First approach is to increment the pointer and return
	///	memory pointer assumming no free function

	///	Updating heap metadata
	heap_t->free -= size;

	uint8_t*	vptr = heap_t->heap_bytes;
	heap_t->heap_bytes += size;
	return vptr;
}
