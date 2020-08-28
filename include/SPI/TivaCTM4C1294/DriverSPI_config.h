#ifndef DRIVERSPI_CONFIG_H_
#define DRIVERSPI_CONFIG_H_

#ifdef __cplusplus
    extern "C"
    {
#endif

    #include <stdint.h>
    #ifndef SYSTEM_CLOCK
    #   define SYSTEM_CLOCK    120000000U
    #endif

    #define DriverSPI_DEFAULT_BITRATE   1000000

    typedef enum TM4C1294XL_SPIName {
        TM4C1294XL_SPI2 = 0,
        TM4C1294XL_SPI3,
        TM4C1294XL_SPICOUNT
    } TM4C1294XL_SPIName;

    extern const uint32_t DriverSPI_devices[ TM4C1294XL_SPICOUNT ];


#ifdef __cplusplus
    }
#endif

#endif
