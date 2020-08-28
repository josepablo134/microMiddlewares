#ifndef DRIVERSPI_H_
#define DRIVERSPI_H_

#ifdef __cplusplus
    extern "C"
    {
#endif

    /// Dependencias locales
    #include <stdbool.h>
    #include <stdint.h>

    /// This as a return should represent a failure
    #define DriverSPI_ERROR                         (-1)

    /// This is not a IOCTL command
    #define DriverSPI_IOCTL_CMD_MASK                0xFFFFFF00

    /// This is not a IOCTL command
    #define DriverSPI_IOCTL_DATA_MASK             (~0xFFFFFF00)

    /// Configure the SPI mode
    //  Requires a struct DriverSPI_mode
    #define DriverSPI_IOCTL_CONF_MODE               0x00000100

    /// Possible SPI Polarities
    #define DriverSPI_SPI_MODE_POL_0                0x00
    #define DriverSPI_SPI_MODE_POL_1                0x01

    /// Possible SPI Phases
    #define DriverSPI_SPI_MODE_PHA_0                0x00
    #define DriverSPI_SPI_MODE_PHA_1                0x01

    struct DriverSPI_mode{
        uint32_t    bitrate;
        uint8_t     polarity;/// DriverSPI_SPI_MODE_POL_{0,1}
        uint8_t     phase;/// DriverSPI_SPI_MODE_PHA_{0,1}
    };

    extern void DriverSPI_init(void);
    extern int DriverSPI_open( unsigned int spi_device);
    extern int DriverSPI_ioctl( int fd , unsigned int config , void* buffer );
    extern void DriverSPI_close( int fd );
    extern int DriverSPI_write( int fd, const void* buffer, unsigned int size);
    extern int DriverSPI_read( int fd, void* buffer, unsigned int size);

#ifdef __cplusplus
    }
#endif

#endif
