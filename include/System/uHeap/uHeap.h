/*!
*	@file		uHeap.h
*	@version	0.0
*	@author		Josepablo Cruz Baas
*	@date		09/07/2020
*	@brief		micro heap declarations.
*	
*	Example of use:
*	@code{.c}
*		#define UHEAP_SIZE N
*		unsigned char heap_bytes[ UHEAP_SIZE ];
*		struct uheap* heap;
*		heap = uheap_init( heap_bytes, UHEAP_SIZE );
*		float* a = uheap_malloc( heap , sizeof(float) );
*		float* b = uheap_malloc( heap , sizeof(float) );
*		float* c = uheap_malloc( heap , sizeof(float) );
*		*a = 2.0;
*		*b = 1.0;
*		*c = *a + *b;
*	@endcode
**/
#ifndef U_HEAP_H_
#define U_HEAP_H_

#ifdef __cplusplus
	extern "C"
	{
#endif

	#include <stdint.h>
	
	/*!
	*	@struct uheap
	*
	*	@brief Heap management structure.
	**/
	struct uheap{
		///Available bytes in the buffer.
		uint16_t	free;
		///Size of the heap.
		uint16_t	length;
		///Heap bytes array.
		uint8_t*	heap_bytes;
	};

	/*!
	*	@brief Initialize and create a new management object.
	*
	*	@param *heap_buffer Pointer to heap management object.
	*	@param heap_size number of available bytes in the heap.
	*
	*	@return struct uheap* pointer to a new heap management object.
	**/
	struct uheap* uheap_init( uint8_t* heap_buffer , uint16_t heap_size );

	/*!
	*	@brief Allocate memory from micro heap through management object.
	*
	*	@param *heap_t pointer to heap management object.
	*	@param size length of bytes required.
	*
	*	@return unsigned char* pointer to byte array allocated or 0x00 in case of error.
	**/
	uint8_t*	uheap_malloc( struct uheap* heap_t, uint16_t size );

#ifdef __cplusplus
	}
#endif

#endif
