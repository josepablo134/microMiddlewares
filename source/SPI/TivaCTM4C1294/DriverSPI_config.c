/// Include Middleware description
//  requires a compilation environment that includes
//  the directory "include/"
#include <SPI/DriverSPI.h>
#include <SPI/TivaCTM4C1294/DriverSPI_config.h>

#include <inc/hw_memmap.h>

const uint32_t DriverSPI_devices[ TM4C1294XL_SPICOUNT ]={
    SSI2_BASE,
    SSI3_BASE
};

uint8_t DriverSPI_flags[ TM4C1294XL_SPICOUNT ]={0x00,0x00};
