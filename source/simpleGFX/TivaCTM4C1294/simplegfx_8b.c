/// Include Middleware description
//  requires a compilation environment that includes
//  the directory "include/"
#include <simpleGFX/simplegfx.h>
#include <DEBUG.h>
#include <stdlib.h>

#ifdef	GFX_8BIT_MODE

void gfx_drawPixel( framebuffer_t* fb , uint16_t x , uint16_t y , uint32_t value ){
	ASSERT( fb != 0x00 );
	ASSERT( fb->buffer != 0x00 );
	ASSERT( fb->width != 0x00 );
	ASSERT( fb->heigth != 0x00 );
	ASSERT( fb->pages != 0x00 );
	ASSERT( fb->columns != 0x00 );

	#ifdef GFX_AVOID_OVERFLOW_REMAPPING
		if( (x > fb->width) || (y > fb->height) ){
			return;
		}
	#endif

	/// Get Segment and Page coordinate

	uint8_t		col = x % fb->columns;
	uint8_t		page = y % fb->pages;
	uint8_t     *pvPage = fb->buffer + ( (page * fb->columns) + (col) );

	/// Write pixel
	*pvPage = (uint8_t)( 0xFF & value);
}

void gfx_fillScreen( framebuffer_t* fb , uint32_t value ){
	ASSERT( fb != 0x00 );
	ASSERT( fb->buffer != 0x00 );

	uint8_t		*pvStart = fb->buffer;
    uint8_t     *pvStop = fb->buffer + ( fb->buffLength - 1 );
	while( pvStart < pvStop ){
		*pvStart = (uint8_t)( 0xFF & value);
		*pvStop = (uint8_t)( 0xFF & value);
		pvStart ++;
		pvStop --;
	}
}

///	Needs a review to convert from 1b to 8b
void gfx_drawBitmap( framebuffer_t* fb, uint16_t x, uint16_t y,
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
