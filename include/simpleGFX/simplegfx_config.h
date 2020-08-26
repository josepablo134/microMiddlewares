#ifndef GFX_CONFIG_H_
#define GFX_CONFIG_H_

#ifdef __cplusplus
    extern "C"
    {
#endif

	///	Enable data format
	#define	GFX_1BIT_MODE
	//#define	GFX_8BIT_MODE
	//#define	GFX_16BIT_MODE
	//#define	GFX_18BIT_MODE

    /// Avoid overflow and underflow remapping
	#define GFX_AVOID_OVERFLOW_REMAPPING

    /// Not related with GFX Driver
    //  User defined values
    #define GFX_WIDTH        (128)
    #define GFX_HEIGHT       (32)
    #define GFX_PIXELS       (GFX_HEIGTH * GFX_WIDTH)
    #define GFX_COLUMNS      GFX_WIDTH
    #define GFX_PAGES        (GFX_HEIGHT / 8)
    #define GFX_BUFFER       (GFX_COLUMNS * GFX_PAGES)

#ifdef __cplusplus
    }
#endif

#endif /* GFX_GFX_CONFIG_H_ */
