/// Include Middleware description
//  requires a compilation environment that includes
//  the directory "include/"
#include <simpleGFX/simpleGFX.h>
#include <DEBUG.h>
#include <stdlib.h>

void gfx_init( struct framebuffer_t* fb ){
	ASSERT( fb != 0x00 );
	fb->buffer = 0x00;
	fb->buffLength = 0x00;
	fb->height = 0x00;
	fb->width = 0x00;

    #ifdef GFX_MULTIMODE_ENABLED
	fb->mode = GFX_MODE_1B;
    #endif
}

void gfx_merge( struct framebuffer_t* fbDest , struct framebuffer_t* fbA , struct framebuffer_t* fbB ){
	ASSERT( fbDest != 0x00 );
	ASSERT( fbA != 0x00 );
	ASSERT( fbB != 0x00 );
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

void gfx_drawHLine( struct framebuffer_t* fb , uint16_t x , uint16_t y , uint16_t length , uint32_t value ){
    uint16_t        xcoor = x;
    uint16_t        len = length;
    while( len-- ){
        /// Assuming overflow control in drawPixel
        gfx_drawPixel( fb , xcoor++ , y , value );
    }
}

void gfx_drawVLine( struct framebuffer_t* fb , uint16_t x , uint16_t y , uint16_t length , uint32_t value ){
    uint16_t        ycoor = y;
    uint16_t        len = length;
    while( len-- ){
        /// Assuming overflow control in drawPixel
        gfx_drawPixel( fb , x , ycoor++ , value );
    }
}

void gfx_drawLine( struct framebuffer_t* fb , uint16_t x0 , uint16_t y0 , uint16_t x1 , uint16_t y1 , uint32_t value ){
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

	/// In favor of reading and optimization
	//  the code were split instead of
	//  variables swapping.
	switch( ylen > xlen ){
	    case true:
	{
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
	case false:
	{
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
	}
	}
}

void gfx_drawTriangle( struct framebuffer_t* fb , uint16_t x0 , uint16_t y0 , uint16_t x1 , uint16_t y1 ,
									uint16_t x2 , uint16_t y2 , uint32_t value ){
	gfx_drawLine( fb , x0 , y0 , x1 , y1 , value );
	gfx_drawLine( fb , x0 , y0 , x2 , y2 , value );
	gfx_drawLine( fb , x1 , y1 , x2 , y2 , value );
}

void gfx_drawRect( struct framebuffer_t* fb , uint16_t x , uint16_t y , uint16_t w , uint16_t h , uint32_t value ){
	gfx_drawHLine( fb , x , y , w , value );
	gfx_drawHLine( fb , x , y + h - 1 , w , value );

	gfx_drawVLine( fb , x , y , h , value );
	gfx_drawVLine( fb , x + w - 1 , y , h , value );
}

void gfx_drawCircle( struct framebuffer_t* fb , uint16_t x0 , uint16_t y0 , uint16_t r , uint32_t value ){
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

