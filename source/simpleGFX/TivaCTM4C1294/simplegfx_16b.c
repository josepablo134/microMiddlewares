/// Include Middleware description
//  requires a compilation environment that includes
//  the directory "include/"
#include <simpleGFX/simplegfx.h>
#include <DEBUG.h>
#include <stdlib.h>

#ifdef	GFX_16BIT_MODE

void gfx_init( framebuffer_t* fb ){
	ASSERT( fb == 0x00 );
    fb->buffer = 0x00;
    fb->buffLength = 0x00;
    fb->height = 0x00;
    fb->width = 0x00;
}

void gfx_merge( framebuffer_t* fbDest , framebuffer_t* fbA , framebuffer_t* fbB ){
	ASSERT( fbDest == 0x00 );
	ASSERT( fbA == 0x00 );
	ASSERT( fbB == 0x00 );
	if( fbDest->buffLength != fbA->buffLength ||
		fbDest->buffLength != fbB->buffLength )
	{return;}
	uint8_t		*pvStart = fbDest->buffer;
    uint8_t     *pvStop = fbDest->buffer + ( fbDest->buffLength - 1 );

	uint8_t		*pvAStart = fbA->buffer;
    uint8_t     *pvAStop = fbA->buffer + ( fbDest->buffLength - 1 );

	uint8_t		*pvBStart = fbB->buffer;
    uint8_t     *pvBStop = fbB->buffer + ( fbDest->buffLength - 1 );
	while( pvStart < pvStop ){
		*pvStart = (*pvAStart) | (*pvBStart);
		*pvStop  = (*pvAStop) | (*pvBStop);
		pvAStart ++;
		pvAStop --;
		pvBStart ++;
		pvBStop --;
		pvStart ++;
		pvStop --;
	}
}

void gfx_drawPixel( framebuffer_t* fb , uint16_t x , uint16_t y , uint16_t value ){
	ASSERT( fb == 0x00 );
	ASSERT( fb->buffer == 0x00 );

	#ifdef GFX_AVOID_OVERFLOW
		if( (x > fb->width) || (y > fb->height) ){
			return;
		}
	#endif

	/// Get Segment and Page coordinate

	uint8_t		segmnt = x % GFX_SEGMENTS;
	uint8_t		page = (y / GFX_PAGES) % GFX_PAGES;
	uint8_t     *pvPage = fb->buffer + ( (page*GFX_SEGMENTS) + (segmnt) );

	/// Get bit shift
	page = y % GFX_PAGES;
	uint8_t     val = 0x01 << ( page );

	/// Write pixel
	*pvPage = (value)? *pvPage | val : *pvPage & ~val;
}

void gfx_fillScreen( framebuffer_t* fb , uint16_t value ){
	ASSERT( fb == 0x00 );
	ASSERT( fb->buffer == 0x00 );
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

void gfx_drawHLine( framebuffer_t* fb , uint16_t x , uint16_t y , uint16_t length , uint16_t value ){
    uint16_t        xcoor = x;
    uint16_t        len = length;
    while( len-- ){
        /// Assuming overflow control in drawPixel
        gfx_drawPixel( fb , xcoor++ , y , value );
    }
}

void gfx_drawVLine( framebuffer_t* fb , uint16_t x , uint16_t y , uint16_t length , uint16_t value ){
    uint16_t        ycoor = y;
    uint16_t        len = length;
    while( len-- ){
        /// Assuming overflow control in drawPixel
        gfx_drawPixel( fb , x , ycoor++ , value );
    }
}

void gfx_drawLine( framebuffer_t* fb , uint16_t x0 , uint16_t y0 , uint16_t x1 , uint16_t y1 , uint16_t value ){
	if( (x0+y0) >= (x1+y1) ){
	///	Swap values if required
		register uint16_t	temp;
		temp = x0;
			x0 = x1;
			x1 = temp;
		temp = y0;
			y0 = y1;
			y1 = temp;
	}
	uint16_t	ylen = (y1>y0)? (y1-y0) : (y0-y1);
	uint16_t	xlen = (x1>x0)? (x1-x0) : (x0-x1);
	int16_t	    xinit,xend;
	int16_t     yinit,yend;
	int16_t		error;

	if( ylen > xlen ){ goto STEEP; }

	/// Guaranteeing xstep incremental
	if( x0 > x1 ){
		xinit = x1; xend = x0;
		yinit = y1;
	}else{
		xinit = x0; xend = x1;
		yinit = y0;
	}
	int8_t      ystep = (y1>=y0)? 1 : -1;
	error = xlen;
	do{
        gfx_drawPixel( fb, xinit++ , yinit, value);
		error -= ylen;
        if( error>0 ){ continue; }
		error += xlen;
        yinit += ystep;
	}while( xinit < xend );

	return;/// End algorithm

STEEP:
    /// Guaranteeing ystep incremental
    if( y0 > y1 ){
        xinit = x1;
        yinit = y1; yend = y0;
    }else{
        xinit = x0;
        yinit = y0; yend = y1;
    }
    int8_t      xstep = (x1>=x0)? 1 : -1;
    error = ylen;
    do{
        gfx_drawPixel( fb, xinit , yinit++, value);
		error -= xlen;
        if( error>0 ){ continue; }
		error += ylen;
        xinit += xstep;
    }while( yinit < yend );

    return;/// End algorithm
}

void gfx_drawTriangle( framebuffer_t* fb , uint16_t x0 , uint16_t y0 , uint16_t x1 , uint16_t y1 ,
									uint16_t x2 , uint16_t y2 , uint16_t value ){
	gfx_drawLine( fb , x0 , y0 , x1 , y1 , value );
	gfx_drawLine( fb , x0 , y0 , x2 , y2 , value );
	gfx_drawLine( fb , x1 , y1 , x2 , y2 , value );
}

void gfx_drawRect( framebuffer_t* fb , uint16_t x , uint16_t y , uint16_t w , uint16_t h , uint16_t value ){
	gfx_drawHLine( fb , x , y , w , value );
	gfx_drawHLine( fb , x , y + h - 1 , w , value );

	gfx_drawVLine( fb , x , y , h , value );
	gfx_drawVLine( fb , x + w - 1 , y , h , value );
}

void gfx_drawCircle( framebuffer_t* fb , uint16_t x0 , uint16_t y0 , uint16_t r , uint16_t value ){
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  gfx_drawPixel( fb, x0, y0 + r, value);
  gfx_drawPixel( fb, x0, y0 - r, value);
  gfx_drawPixel( fb, x0 + r, y0, value);
  gfx_drawPixel( fb, x0 - r, y0, value);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    gfx_drawPixel( fb, x0 + x, y0 + y, value);
    gfx_drawPixel( fb, x0 - x, y0 + y, value);
    gfx_drawPixel( fb, x0 + x, y0 - y, value);
    gfx_drawPixel( fb, x0 - x, y0 - y, value);
    gfx_drawPixel( fb, x0 + y, y0 + x, value);
    gfx_drawPixel( fb, x0 - y, y0 + x, value);
    gfx_drawPixel( fb, x0 + y, y0 - x, value);
    gfx_drawPixel( fb, x0 - y, y0 - x, value);
  }
}

void gfx_drawBitmap( framebuffer_t* fb, uint16_t x, uint16_t y,
					 const uint8_t bitmap[], uint16_t w, uint16_t h, uint16_t value ) {
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
