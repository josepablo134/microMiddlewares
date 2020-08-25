#ifndef GFX_CONFIG_H_
#define GFX_CONFIG_H_

#ifdef __cplusplus
    extern "C"
    {
#endif

	#define	GFX_1BIT_MODE
	//#define	GFX_8BIT_MODE
	//#define	GFX_16BIT_MODE
    #define GFX_WIDTH        (128)
    #define GFX_HEIGHT       (64)
    #define GFX_PIXELS       (GFX_HEIGTH * GFX_WIDTH)
	#define GFX_SEGMENTS	 (128)
	#define GFX_PAGES		 (8)
	#define	GFX_BUFFER		 (GFX_SEGMENTS * GFX_PAGES)

    /// Avoid overflow and underflow writting
	#define GFX_AVOID_OVERFLOW

	/*
	*	Type of framebuffer depends on implementation of
	*	display.
	**/

	#ifdef	GFX_1BIT_MODE
			typedef struct framebuffer_t{
				uint16_t	height,width;
				uint16_t    buffLength;
				uint8_t     *buffer;
			}framebuffer_t;
	#endif
	#ifdef	GFX_8BIT_MODE
			typedef struct framebuffer_t{
				uint16_t	height,width;
				uint16_t    buffLength;
				uint8_t     *buffer;
			}framebuffer_t;
	#endif
	#ifdef	GFX_16BIT_MODE
			typedef struct framebuffer_t{
				uint16_t	height,width;
				uint16_t    buffLength;
				uint16_t    *buffer;
			}framebuffer_t;
	#endif

#ifdef __cplusplus
    }
#endif

#endif /* GFX_GFX_CONFIG_H_ */
