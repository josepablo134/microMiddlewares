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

    /// SPI devices software flags
    #define DriverSPI_MODE_MASK             0x01 /* Frame mode mask  */
    #define DriverSPI_MODE_FRAME_NORMAL     0x00 /* Each write/read operation is a Frame */
    #define DriverSPI_MODE_FRAME_PARTITION  0x01 /* The Frame is divided by multiple readings/writings */

    typedef enum TM4C1294XL_SPIName {
        TM4C1294XL_SPI2 = 0,
        TM4C1294XL_SPI3,
        TM4C1294XL_SPICOUNT
    } TM4C1294XL_SPIName;

    extern const uint32_t DriverSPI_devices[ TM4C1294XL_SPICOUNT ];
    extern uint8_t DriverSPI_flags[ TM4C1294XL_SPICOUNT ];


#ifdef __cplusplus
    }
#endif

#endif
