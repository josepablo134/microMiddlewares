#ifndef GFX_CONFIG_H_
#define GFX_CONFIG_H_

#ifdef __cplusplus
    extern "C"
    {
#endif


    #define GFX_MULTIMODE_ENABLED 1
//  #define GFX_1BIT_MODE 1
//  #define GFX_8BIT_MODE 1
//  #define GFX_16BIT_MODE 1
//  #define GFX_18BIT_MODE 1

    /// Avoid overflow and underflow remapping
    #define GFX_AVOID_OVERFLOW_REMAPPING

    /*
    *   Type of framebuffer depends on implementation of
    *   display.
    **/
    #ifdef GFX_MULTIMODE_ENABLED

        typedef enum framebuffer_bits_mode{
            GFX_MODE_1B=0,
            GFX_MODE_8B,
            GFX_MODE_16B,
            GFX_MODES_COUNT
        }framebuffer_bits_mode;

        struct framebuffer_t{
            uint32_t    buffLength;
            uint16_t    height,width;
            framebuffer_bits_mode mode;
            uint8_t     *buffer;
        };
    #else
        struct framebuffer_t{
            uint32_t    buffLength;
            uint16_t    height,width;
            uint8_t     *buffer;
        };
    #endif


    #ifdef GFX_MULTIMODE_ENABLED
        extern void gfx_drawPixel_1bit( struct framebuffer_t* fb , uint16_t x , uint16_t y , uint32_t value );
        extern void gfx_fillScreen_1bit( struct framebuffer_t* fb , uint32_t value );
        extern void gfx_drawBitmap_1bit( struct framebuffer_t* fb, uint16_t x, uint16_t y,
                                         const uint8_t bitmap[], uint16_t w, uint16_t h, uint32_t value );

        extern void gfx_drawPixel_8bit( struct framebuffer_t* fb , uint16_t x , uint16_t y , uint32_t value );
        extern void gfx_fillScreen_8bit( struct framebuffer_t* fb , uint32_t value );
        extern void gfx_drawBitmap_8bit( struct framebuffer_t* fb, uint16_t x, uint16_t y,
                                         const uint8_t bitmap[], uint16_t w, uint16_t h, uint32_t value );

        extern void gfx_drawPixel_16bit( struct framebuffer_t* fb , uint16_t x , uint16_t y , uint32_t value );
        extern void gfx_fillScreen_16bit( struct framebuffer_t* fb , uint32_t value );
        extern void gfx_drawBitmap_16bit( struct framebuffer_t* fb, uint16_t x, uint16_t y,
                                          const uint8_t bitmap[], uint16_t w, uint16_t h, uint32_t value );
    #endif

    #ifdef GFX_1BIT_MODE
        extern void gfx_drawPixel_1bit( struct framebuffer_t* fb , uint16_t x , uint16_t y , uint32_t value );
        extern void gfx_fillScreen_1bit( struct framebuffer_t* fb , uint32_t value );
        extern void gfx_drawBitmap_1bit( struct framebuffer_t* fb, uint16_t x, uint16_t y,
                                         const uint8_t bitmap[], uint16_t w, uint16_t h, uint32_t value );
    #endif

    #ifdef GFX_8BIT_MODE
        extern void gfx_drawPixel_8bit( struct framebuffer_t* fb , uint16_t x , uint16_t y , uint32_t value );
        extern void gfx_fillScreen_8bit( struct framebuffer_t* fb , uint32_t value );
        extern void gfx_drawBitmap_8bit( struct framebuffer_t* fb, uint16_t x, uint16_t y,
                                         const uint8_t bitmap[], uint16_t w, uint16_t h, uint32_t value );
    #endif

    #ifdef GFX_16BIT_MODE
        extern void gfx_drawPixel_16bit( struct framebuffer_t* fb , uint16_t x , uint16_t y , uint32_t value );
        extern void gfx_fillScreen_16bit( struct framebuffer_t* fb , uint32_t value );
        extern void gfx_drawBitmap_16bit( struct framebuffer_t* fb, uint16_t x, uint16_t y,
                                          const uint8_t bitmap[], uint16_t w, uint16_t h, uint32_t value );
    #endif

#ifdef __cplusplus
    }
#endif

#endif /* GFX_GFX_CONFIG_H_ */
