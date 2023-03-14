#include <simpleGFX/simpleGFX.h>

#ifdef GFX_MULTIMODE_ENABLED
    typedef void (*drawPixelCallback)( struct framebuffer_t* fb , uint16_t x , uint16_t y , uint32_t value );
    typedef void (*fillScreenCallback)( struct framebuffer_t* fb , uint32_t value );
    typedef void (*drawBitmapCallback)( struct framebuffer_t* fb, uint16_t x, uint16_t y,
                const uint8_t bitmap[], uint16_t w, uint16_t h, uint32_t value );

    drawPixelCallback   drawPixelCallback_array[GFX_MODES_COUNT]= {
           gfx_drawPixel_1bit,
           gfx_drawPixel_8bit,
           gfx_drawPixel_16bit
    };
    fillScreenCallback fillScreenCallback_array[GFX_MODES_COUNT]={
           gfx_fillScreen_1bit,
           gfx_fillScreen_8bit,
           gfx_fillScreen_16bit
    };

    drawBitmapCallback drawBitmapCallback_array[GFX_MODES_COUNT]={
           gfx_drawBitmap_1bit,
           gfx_drawBitmap_8bit,
           gfx_drawBitmap_16bit,
    };

void gfx_drawPixel( struct framebuffer_t* fb , uint16_t x , uint16_t y , uint32_t value ){
    return drawPixelCallback_array[ fb->mode ]( fb , x , y , value );
}
void gfx_fillScreen( struct framebuffer_t* fb , uint32_t value ){
    return fillScreenCallback_array[ fb->mode ]( fb , value );
}
void gfx_drawBitmap( struct framebuffer_t* fb, uint16_t x, uint16_t y,
                                const uint8_t bitmap[], uint16_t w, uint16_t h, uint32_t value ){
    return drawBitmapCallback_array[ fb->mode ]( fb, x, y, bitmap, w, h, value );
}
#else

    #ifdef  GFX_1BIT_MODE
        void gfx_drawPixel( struct framebuffer_t* fb , uint16_t x , uint16_t y , uint32_t value ){
            return gfx_drawPixel_1bit( fb , x , y , value );
        }
        void gfx_fillScreen( struct framebuffer_t* fb , uint32_t value ){
            return gfx_fillScreen_1bit( fb , value );
        }
        void gfx_drawBitmap( struct framebuffer_t* fb, uint16_t x, uint16_t y,
                                   const uint8_t bitmap[], uint16_t w, uint16_t h, uint32_t value ){
            return gfx_drawBitmap_1bit( fb, x, y, bitmap, w, h, value );
        }
    #endif

    #ifdef  GFX_8BIT_MODE
        void gfx_drawPixel( struct framebuffer_t* fb , uint16_t x , uint16_t y , uint32_t value ){
            return gfx_drawPixel_8bit( fb , x , y , value );
        }
        void gfx_fillScreen( struct framebuffer_t* fb , uint32_t value ){
            return gfx_fillScreen_8bit( fb , value );
        }
        void gfx_drawBitmap( struct framebuffer_t* fb, uint16_t x, uint16_t y,
                                   const uint8_t bitmap[], uint16_t w, uint16_t h, uint32_t value ){
            return gfx_drawBitmap_8bit( fb, x, y, bitmap, w, h, value );
        }
    #endif

    #ifdef  GFX_16BIT_MODE
        void gfx_drawPixel( struct framebuffer_t* fb , uint16_t x , uint16_t y , uint32_t value ){
            return gfx_drawPixel_16bit( fb , x , y , value );
        }
        void gfx_fillScreen( struct framebuffer_t* fb , uint32_t value ){
            return gfx_fillScreen_16bit( fb , value );
        }
        void gfx_drawBitmap( struct framebuffer_t* fb, uint16_t x, uint16_t y,
                                   const uint8_t bitmap[], uint16_t w, uint16_t h, uint32_t value ){
            return gfx_drawBitmap_16bit( fb, x, y, bitmap, w, h, value );
        }
    #endif
#endif
