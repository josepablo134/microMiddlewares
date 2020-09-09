/*!
*	@file		DriverSPI.h
*	@version	0.0
*	@author		Josepablo Cruz Baas
*	@date		08/26/2020
*	@brief		DriverSPI declaration of non specific interfaces.
*
*	Example of use:
*	@code{.c}
*		int fd;
*		unsigned char buffer[] = {0x00,0x01,0x02};
*		DriverSPI_init();
*		fd = DriverSPI_open( 0 );
*		DriverSPI_write( fd , buffer , sizeof( buffer ) );
*	@endcode
**/
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
    #define DriverSPI_IOCTL_DATA_MASK             (~DriverSPI_IOCTL_CMD_MASK)

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

	//! @brief Initialize and clear driver registers.
    extern void DriverSPI_init(void);
	
	/*!
	*	@brief Acquire file descriptor of the peripheral.
	*
	*	@param spi_device device or peripheral to acquire.
	*
	*	@return int fd, file descriptor of the peripheral or -1 in case or error.
	**/
    extern int DriverSPI_open( unsigned int spi_device);
	
	/*!
	*	@brief Release the file descriptor
	**/
    extern void DriverSPI_close( int fd );

	/*!
	*	@brief IO control command or configuration.
	*
	*	@param fd file descriptor that represents the peripheral.
	*	@param config integer bit mask that represents the io operation.
	*	@param *buffer a byte array that holds any shareable buffer depending on io operation.
	*
	*	@return DriverSPI_ERROR or 0 in case of success.
	**/
    extern int DriverSPI_ioctl( int fd , unsigned int config , void* buffer );
	
	/*!
	*	@brief Transmiting bytes over SPI bus.
	*
	*	@param fd file descriptor that represents the peripheral.
	*	@param buffer byte array that holds all data ready to transmit.
	*	@param size number of bytes to transmit
	*	
	*	@return int, number of bytes transmitted.
	**/
    extern int DriverSPI_write( int fd, const void* buffer, unsigned int size);

	/*!
	*	@brief Received bytes over SPI bus.
	*
	*	@param fd file descriptor that represents the peripheral.
	*	@param buffer byte array where save all data received.
	*	@param size number of bytes to read.
	*
	*	@return int, number of bytes received.
	**/
    extern int DriverSPI_read( int fd, void* buffer, unsigned int size);

#ifdef __cplusplus
    }
#endif

#endif
