#ifndef GFX_H_
#define GFX_H_

#ifdef __cplusplus
    extern "C"
    {
#endif

	#include <stdbool.h>
    #include <stdint.h>
    #include <simpleGFX/simpleGFX_config.h>

    extern void gfx_init( struct framebuffer_t* fb );

	extern void gfx_fillScreen( struct framebuffer_t* fb , uint32_t value );
	
	extern void gfx_merge( struct framebuffer_t* fbDest , struct framebuffer_t* fbA , struct framebuffer_t* fbB );

	extern void gfx_drawPixel( struct framebuffer_t* fb , uint16_t x , uint16_t y , uint32_t value );

	extern void gfx_drawHLine( struct framebuffer_t* fb , uint16_t x , uint16_t y , uint16_t length , uint32_t value );

	extern void gfx_drawVLine( struct framebuffer_t* fb , uint16_t x , uint16_t y , uint16_t length , uint32_t value );

	extern void gfx_drawLine( struct framebuffer_t* fb , uint16_t x0 , uint16_t y0 , uint16_t x1 , uint16_t y1 , uint32_t value );

	extern void gfx_drawTriangle( struct framebuffer_t* fb , uint16_t x0 , uint16_t y0 , uint16_t x1 , uint16_t y1 ,
									uint16_t x2 , uint16_t y2 , uint32_t value );

	extern void gfx_drawRect( struct framebuffer_t* fb , uint16_t x , uint16_t y , uint16_t w , uint16_t h , uint32_t value );

	extern void gfx_drawCircle( struct framebuffer_t* fb , uint16_t x0 , uint16_t y0 , uint16_t r , uint32_t value );

	extern void gfx_drawBitmap( struct framebuffer_t* fb, uint16_t x, uint16_t y, const uint8_t bitmap[],
								uint16_t w, uint16_t h, uint32_t value);

#ifdef __cplusplus
    }
#endif

#endif
