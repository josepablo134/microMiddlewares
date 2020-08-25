/// Include Middleware description
//  requires a compilation environment that includes
//  the directory "include/"
#include <I2C/DriverI2C_config.h>
#include <inc/hw_memmap.h>

const uint32_t DriverI2C_devices[EK_TM4C1294XL_I2CCOUNT]={
	I2C7_BASE,
	I2C8_BASE
};
