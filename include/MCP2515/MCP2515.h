#ifndef MCP2515_H_
#define MCP2515_H_

#ifdef __cplusplus
    extern "C"
    {
#endif

	/// Dependencias locales
	#include <stdbool.h>
	#include <stdint.h>

    /// This as a return should represent a failure
	#define MCP2515_ERROR                         (-1)

    /// This is not a IOCTL command
    #define MCP2515_IOCTL_CMD_MASK            	0xFFFFFF00

	///	This is not a IOCTL command
    #define MCP2515_IOCTL_DATA_MASK            	0x000000FF

	///	Initialize controller registers
	extern void MCP2515_init(void);
	///	Prepare controller to any transaction
	extern int MCP2515_open(unsigned int device );
	///	Send a IOCTL order
	extern int MCP2515_ioctl( int fd , unsigned int config , void* buffer );
	///	Send a new package
	extern int MCP2515_write( int fd , const void* buffer, unsigned int size );
	///	Read any available package
	extern int MCP2515_read( int fd , void* buffer, unsigned int size );
	///	Release controller
	extern void MCP2515_close( int fd );

#ifdef __cplusplus
    }
#endif

#endif
