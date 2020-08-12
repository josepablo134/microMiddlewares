#ifndef DriverI2C_H_
#define DriverI2C_H_
	/// Dependencias locales
	#include <stdbool.h>
	#include <stdint.h>
	#define DriverI2C_ERROR     (-1)

    #define DriverI2C_IOCTL_CMD_MASK            	0xFFFFFF00
    #define DriverI2C_IOCTL_CMD_SETADDR         	0x00000100
    #define DriverI2C_IOCTL_CMD_SETSPEED_100K   	0x00000200
    #define DriverI2C_IOCTL_CMD_SETSPEED_400K   	0x00000300
	#define DriverI2C_IOCTL_CMD_STOP_TRANSACTION	0x00000400
    #define DriverI2C_IOCTL_CMD_SEND_BYTE           0x00000500
    #define DriverI2C_IOCTL_CMD_RECEIVE_BYTE        0x00000600
    #define DriverI2C_IOCTL_CMD_READ_REG            0x00000700

	/// Initialize i2c buffer
	extern void DriverI2C_init(void);
	///	Acquire a file descriptor for i2c
	extern int8_t DriverI2C_open( uint8_t i2c_device);
	/// Configure the i2c device
	extern int8_t DriverI2C_ioctl( int8_t fd , uint32_t config , void* buffer );
	///	Release a file descriptor for i2c
	extern void DriverI2C_close( int8_t fd );
	extern int32_t DriverI2C_write( int8_t fd, const void* buffer, uint32_t size);
	extern int32_t DriverI2C_read( int8_t fd, void* buffer, uint32_t size);
	extern int32_t DriverI2C_writeRegister( int8_t fd, uint8_t address, const void* buffer, uint32_t size);
	extern int32_t DriverI2C_readRegister( int8_t fd, uint8_t address, void* buffer, uint32_t size);
	extern bool DriverI2C_isBusy( int8_t fd );
#endif
