#ifndef Driver8080_H_
#define Driver8080_H_

#ifdef __cplusplus
    extern "C"
    {
#endif

	/// Dependencias locales
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

	extern void Driver8080_init(void);
	extern int Driver8080_open(void);
	extern int Driver8080_ioctl( unsigned int config , void* buffer );
	extern void Driver8080_close(void);
	extern int Driver8080_write( const void* buffer, unsigned int size );
	extern int Driver8080_read( void* buffer, unsigned int size );

#ifdef __cplusplus
    }
#endif

#endif
