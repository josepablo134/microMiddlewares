#ifndef MIDDLEWARES_DRIVERSPI_H_
#define MIDDLEWARES_DRIVERSPI_H_

#ifdef __cplusplus
    extern "C"
    {
#endif

    /// Dependencias locales
    #include <stdbool.h>
    #include <stdint.h>

    extern void DriverSPI_init(void);
    extern int DriverSPI_open( unsigned int i2c_device);
    extern int DriverSPI_ioctl( int fd , unsigned int config , void* buffer );
    extern void DriverSPI_close( int fd );
    extern int DriverSPI_write( int fd, const void* buffer, unsigned int size);
    extern int DriverSPI_read( int fd, void* buffer, unsigned int size);

#ifdef __cplusplus
    }
#endif

#endif
