#ifndef U_HEAP_H_
#define U_HEAP_H_

#ifdef __cplusplus
	extern "C"
	{
#endif

	#include <stdint.h>
	
	struct uheap{
		uint8_t*	heap_bytes;
		uint16_t	free;
		uint16_t	length;
	};

	struct uheap* uheap_init( uint8_t* heap_buffer , uint16_t heap_size );

	uint8_t*	uheap_malloc( struct uheap* heap_t, uint16_t size );

#ifdef __cplusplus
	}
#endif

#endif
