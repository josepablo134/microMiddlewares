#ifndef DriverI2C_CONFIG_H_
#define DriverI2C_CONFIG_H_

#ifdef __cplusplus
    extern "C"
    {
#endif

    #include <stdint.h>
    #ifndef SYSTEM_CLOCK
	#   define SYSTEM_CLOCK    120000000U
    #endif
	typedef enum EK_TM4C1294XL_I2CName {
		EK_TM4C1294XL_I2C7 = 0,
		EK_TM4C1294XL_I2C8,
		EK_TM4C1294XL_I2CCOUNT
	} EK_TM4C1294XL_I2CName;
	extern const uint32_t DriverI2C_devices[ EK_TM4C1294XL_I2CCOUNT ];

#ifdef __cplusplus
    }
#endif

#endif
