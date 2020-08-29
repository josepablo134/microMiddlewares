#ifndef GFX_CONFIG_H_
#define GFX_CONFIG_H_

#ifdef __cplusplus
    extern "C"
    {
#endif

	///	Enable data format
//	#define	GFX_1BIT_MODE
//	#define	GFX_8BIT_MODE
	#define	GFX_16BIT_MODE
//	#define	GFX_18BIT_MODE

    /// Avoid overflow and underflow remapping
	#define GFX_AVOID_OVERFLOW_REMAPPING

#ifdef __cplusplus
    }
#endif

#endif /* GFX_GFX_CONFIG_H_ */
