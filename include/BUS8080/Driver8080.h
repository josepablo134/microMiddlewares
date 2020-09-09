/*!
*	@file		Driver8080.h
*	@version	0.0
*	@author		Josepablo Cruz Baas
*	@date		08/26/2020
*	@brief		Driver8080 declaration of non specific interfaces
*
*	Example of use:
*	@code{.c}
*		unsigned char buffer[] = {0x00,0x01,0x02};
*		Driver8080_init();
*		Driver8080_open();
*		Driver8080_ioctl( Driver8080_IOCTL_CMD_BEGIN_WR | 0xADDR , buffer );
*		Driver8080_write( buffer , sizeof( buffer ) );
*		Driver8080_ioctl( Driver8080_IOCTL_CMD_RELEASE );
*	@endcode
**/
#ifndef Driver8080_H_
#define Driver8080_H_

#ifdef __cplusplus
    extern "C"
    {
#endif

	#include <stdbool.h>
	#include <stdint.h>

    /// This as a return should represent a failure
	#define Driver8080_ERROR                         (-1)

    /// This is not a IOCTL command
    #define Driver8080_IOCTL_CMD_MASK            	0xFFFFFF00

	///	This is not a IOCTL command
    #define Driver8080_IOCTL_DATA_MASK            	0x000000FF

	///	Release CS signal
	#define Driver8080_IOCTL_CMD_RELEASE			0x00000100

	///	Send a single byte as address and holds CS
	#define Driver8080_IOCTL_CMD_BEGIN_WR			0x00000200

	///	Send a single byte as address and holds CS
	#define Driver8080_IOCTL_CMD_BEGIN_RD			0x00000300

	///	Send a byte array as address and holds CS
	//	requires a byte for address length
	#define Driver8080_IOCTL_CMD_BEGIN_WR_ARR		0x00000400

	///	Send a byte array as address and holds CS
	//	requires a byte for address length
	#define Driver8080_IOCTL_CMD_BEGIN_RD_ARR		0x00000500

	///	Send a rst signal
	#define Driver8080_IOCTL_CMD_RST				0x00000600

	/*!
	*	@brief Initialize and clear driver registers.
	**/
	extern void Driver8080_init(void);
	
	/*!
	*	@brief Acquire a file descriptor for the peripheral.
	*
	*	Configures the peripheral with the default configuration.
	*
	*	@return int, 0 if success, -1 if error.
	**/
	extern int Driver8080_open(void);

	/*!
	*	@brief Close and release the bus
	**/
	extern void Driver8080_close(void);

	/*!
	*	@brief IO control command.
	*
	*	Example of use writting 3 bytes as address through the bus.
	*
	*	@code{.c}
	*		unsigned char buffer[3] = {0x00,0x01,0x02};
	*		Driver8080_ioctl( Driver8080_IOCTL_CMD_BEGIN_WR_ARR | 3 , buffer );
	*	@endcode
	*
	*	@param	unsigned int config, configuration bit mask.
	*	@param	void* buffer, buffer pointer for ioctl transaction.
	*
	*	@return int fd, a file descriptor identitifier.
	**/
	extern int Driver8080_ioctl( unsigned int config , void* buffer );

	/*!
	*	@brief Write bytes through the bus.
	*	
	*	Example of use writting 3 bytes at register 0x00
	*
	*	@code{.c}
	*		unsigned char buffer[] = {0x00,0x01,0x02};
	*		Driver8080_ioctl( Driver8080_IOCTL_CMD_BEGIN_WR | 0x00 , NULL );
	*		Driver8080_write( buffer , sizeof( buffer ) );
	*		Driver8080_ioctl( Driver8080_IOCTL_CMD_RELEASE );
	*	@endcode
	*
	*	@param	void* buffer, buffer pointer for write transaction.
	*	@param	unsigned int size, number of bytes in the buffer.
	*
	*	@return int, number of bytes writen.
	**/
	extern int Driver8080_write( const void* buffer, unsigned int size );

	/*!
	*	@brief Read bytes from the bus.
	*
	*	Example of use assuming register address 0x00
	*
	*	@code{.c}
	*		unsigned char buffer[3];
	*		Driver8080_ioctl( Driver8080_IOCTL_CMD_BEGIN_RD | 0x00 , NULL );
	*		Driver8080_read( buffer , sizeof( buffer ) );
	*		Driver8080_ioctl( Driver8080_IOCTL_CMD_RELEASE );
	*	@endcode
	**/
	extern int Driver8080_read( void* buffer, unsigned int size );

#ifdef __cplusplus
    }
#endif

#endif
