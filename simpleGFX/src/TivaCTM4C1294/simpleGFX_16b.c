/// Include Middleware description
//  requires a compilation environment that includes
//  the directory "include/"
#include <simpleGFX/simpleGFX.h>
#include <DEBUG.h>
#include <stdlib.h>

#if defined ( GFX_16BIT_MODE )|| ( GFX_MULTIMODE_ENABLED )

///	Compute the address for 16bit data
void gfx_drawPixel_16bit( struct framebuffer_t* fb , uint16_t x , uint16_t y , uint32_t value ){
    ASSERT( fb != 0x00 );
    ASSERT( fb->buffer != 0x00 );
    ASSERT( fb->width != 0x00 );
    ASSERT( fb->height != 0x00 );
	ASSERT( (fb->buffLength % 2) != 0x00 );

	#ifdef GFX_AVOID_OVERFLOW_REMAPPING
		if( (x > fb->width) || (y > fb->height) ){
			return;
		}
	#endif

	/// Get Segment and Page coordinate

	uint16_t		col = x % fb->width;
	uint16_t		page = y % fb->height;
	uint32_t        addr = ( (page * fb->width) + (col) );

	((uint16_t*)fb->buffer)[ addr ] = ( 0xFFFF & value );
}

void gfx_fillScreen_16bit( struct framebuffer_t* fb , uint32_t value ){
	ASSERT( fb != 0x00 );
	ASSERT( fb->buffer != 0x00 );

	uint16_t    *pvStart = (uint16_t*)fb->buffer;
    uint16_t    *pvStop = ((uint16_t*)fb->buffer) + ( fb->buffLength >> 1);
	while( pvStart < pvStop ){
		*pvStart = (uint16_t)( 0xFFFF & value);
		*pvStop = (uint16_t)( 0xFFFF & value);
		pvStart ++;
		pvStop --;
	}
}

///	Needs a review to convert from 1b to 8b
void gfx_drawBitmap_16bit( struct framebuffer_t* fb, uint16_t x, uint16_t y,
					 const uint8_t bitmap[], uint16_t w, uint16_t h, uint32_t value ) {
	if( !bitmap ){ return; }
	int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
	uint8_t byte = 0;
	int16_t i,j;
	for (j = 0; j < h; j++, y++) {
		for (i = 0; i < w; i++) {
			if (i & 7){
				byte <<= 1;
			}else{
				byte = bitmap[j * byteWidth + i / 8];
			}
			if (byte & 0x80){
				gfx_drawPixel( fb , x + i, y, value);
			}
		}
	}
}

#endif
