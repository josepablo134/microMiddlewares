/// Include Middleware description
//  requires a compilation environment that includes
//  the directory "include/"
#include <simpleGFX/simplegfx.h>
#include <DEBUG.h>
#include <stdlib.h>

#if defined	( GFX_1BIT_MODE )|| ( GFX_MULTIMODE_ENABLED )

#define GFX_BITS_PER_PAGE           8
#define GFX_BITS_PER_COLUMN         1

void gfx_drawPixel_1bit( struct framebuffer_t* fb , uint16_t x , uint16_t y , uint32_t value ){
	ASSERT( fb != 0x00 );
	ASSERT( fb->buffer != 0x00 );
	ASSERT( fb->width != 0x00 );
	ASSERT( fb->height != 0x00 );

	#ifdef GFX_AVOID_OVERFLOW_REMAPPING
		if( (x > fb->width) || (y > fb->height) ){
			return;
		}
	#endif

	/// Get Segment and Page coordinate
	uint8_t     columns = fb->width / GFX_BITS_PER_COLUMN;
	uint8_t     pages = fb->height / GFX_BITS_PER_PAGE;

	uint8_t		segmnt = (x / GFX_BITS_PER_COLUMN) % (columns);
	uint8_t		page = (y / GFX_BITS_PER_PAGE) % (pages);
	uint8_t     *pvPage = fb->buffer + ( (page * columns) + (segmnt - 1) );

	/// Get bit shift
	page = y % (GFX_BITS_PER_PAGE);
	uint8_t     val = 0x01 << ( page );

	/// Write pixel
	*pvPage = (value)? *pvPage | val : *pvPage & ~val;
}

void gfx_fillScreen_1bit( struct framebuffer_t* fb , uint32_t value ){
	ASSERT( fb != 0x00 );
	ASSERT( fb->buffer != 0x00 );
	uint8_t		val = ((uint8_t)value) * (0xFF);

	uint8_t		*pvStart = fb->buffer;
    uint8_t     *pvStop = fb->buffer + ( fb->buffLength - 1 );
	while( pvStart < pvStop ){
		*pvStart = val;
		*pvStop = val;
		pvStart ++;
		pvStop --;
	}
}

void gfx_drawBitmap_1bit( struct framebuffer_t* fb, uint16_t x, uint16_t y,
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
