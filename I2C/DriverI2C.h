#ifndef DriverI2C_H_
#define DriverI2C_H_

#ifdef __cplusplus
    extern "C"
    {
#endif

	/// Dependencias locales
	#include <stdbool.h>
	#include <stdint.h>

    /// This as a return should represent a failure
	#define DriverI2C_ERROR                         (-1)

    /// This is not a IOCTL command
    #define DriverI2C_IOCTL_CMD_MASK            	0xFFFFFF00

    /// Set a new slave address ( requires a bit OR with 8 bit slave address )
    #define DriverI2C_IOCTL_CMD_SETADDR         	0x00000100

    /// Change i2c speed to 100kbps
    #define DriverI2C_IOCTL_CMD_SETSPEED_100K   	0x00000200

    /// Change i2c speed to 400kbps
    #define DriverI2C_IOCTL_CMD_SETSPEED_400K   	0x00000300

    /// Forcing to send a STOP bit
	#define DriverI2C_IOCTL_CMD_STOP_TRANSACTION	0x00000400

    /// Send a single byte ( the byte must be set with bit OR )
    #define DriverI2C_IOCTL_CMD_SEND_BYTE           0x00000500

    /// Receive a single byte ( a buffer is required to store the result )
    #define DriverI2C_IOCTL_CMD_RECEIVE_BYTE        0x00000600

    /// Send a single byte and receive a single byte
    //  requires a buffer to store  the result
    //  and the transmition byte should be set with a bit OR.
    #define DriverI2C_IOCTL_CMD_READ_REG            0x00000700

    /**
     *  Turns on the peripherals
     */
	extern void DriverI2C_init(void);

	/**
	 *  Acquire a file descriptor for the peripheral.
	 *  Configures the peripheral with the default configuration.
	 *  @param  i2c_device An integer that identifies the i2c peripheral.
	 *  @return An integer file descriptor or DriverI2C_ERROR
	 */
	extern int DriverI2C_open( unsigned int i2c_device );

	/**
	 *  Special configuration can be set or read.
	 *  @param  fd A file descriptor.
	 *  @param  config A 32 bit integer bit mask that represents the  io operation.
	 *  @param  buffer A byte array that holds any sharable buffer depending on io operation.
	 *  @return DriverI2C_ERROR or 0 in case of success.
	 *
	 *  Example of use
	 *
	 *  @code{.c}
     *      uint8_t tempBuffer[] = { 0x00, 0x00, 0x00 };
	 *      uint8_t regAddr = 0x55;
	 *      DriverI2C_ioctl( i2cFd , DriverI2C_IOCTL_CMD_READ_REG | regAddr , tempBuffer );
	 *  @endcode
	 */
	extern int DriverI2C_ioctl( int fd , unsigned int config , void* buffer );

	///	Release a file descriptor
	extern void DriverI2C_close( int fd );

	/**
	 * Send a byte array with length from 1 to 4Gb, the buffer stores the data
	 * and should be persistent while the transaction take place.
	 * In case of success the bus does not return to idle.
	 */
	extern int DriverI2C_write( int fd, const void* buffer, unsigned int size );

	/**
	 * Receive a byte array with length from 1 to 4Gb, the buffer stores the data
	 * and should be persistent while the transaction take place.
     * In case of success the bus does not return to idle.
	 */
	extern int DriverI2C_read( int fd, void* buffer, unsigned int size );

#ifdef __cplusplus
    }
#endif

#endif
