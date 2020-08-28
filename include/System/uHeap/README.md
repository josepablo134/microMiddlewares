# micro Heap

A minimalist dynamic memory system

## Example of use

```
	unsigned char heap_buffer[ USER_DEFINED_MAX_HEAP_SIZE ];
	struct uheap* heap;
	float *a,*b;

	heap = uheap_init( heap_buffer , sizeof(heap_buffer) );
	if( !heap ){
		fprintf(stderr,"Error initializing heap struct");
		return -1;
	}

	a = (float*) uheap_malloc( heap , sizeof(float) );
	b = (float*) uheap_malloc( heap , sizeof(float) );

	*a = 3.1416;
	*b = 3.1614;
	
	fprintf(stdout,"%f + %f = %f\n",*a,*b,(*a + *b));
```
