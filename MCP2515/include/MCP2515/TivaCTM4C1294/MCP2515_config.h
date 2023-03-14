#ifndef MCP2515_CONFIG_H_
#define MCP2515_CONFIG_H_

#ifdef __cplusplus
    extern "C"
    {
#endif

    #include <stdint.h>
    #ifndef SYSTEM_CLOCK
	#   define SYSTEM_CLOCK    120000000U
    #endif

	typedef enum TM4C1294XL_MCP2515Name {
		TM4C1294XL_MCP2515_DEV0 = 0,
		TM4C1294XL_MCP2515_COUNT
	} TM4C1294XL_I2CName;
	extern const uint32_t MCP2515_devices[ TM4C1294XL_MCP2515_COUNT ];

#ifdef __cplusplus
    }
#endif

#endif
