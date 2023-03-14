#ifndef ILI9341_H_
#define ILI9341_H_

#ifdef __cplusplus
    extern "C"
    {
#endif

	/// Dependencias locales
	#include <stdbool.h>
	#include <stdint.h>

    /// This as a return should represent a failure
	#define ILI9341_ERROR                         (-1)

    /// This is not a IOCTL command
    #define ILI9341_IOCTL_CMD_MASK            	0xFFFFFF00

	///	This is not a IOCTL command
    #define ILI9341_IOCTL_DATA_MASK            	0x000000FF

	///	Write command
	#define ILI9341_IOCTL_SEND_CMD              0x00000100

	///	Begin data writing
	#define ILI9341_IOCTL_BEGIN_WR              0x00000200

	///	Begin data reading
	#define ILI9341_IOCTL_BEGIN_RD              0x00000300

	///	Stop reading / writing
	#define ILI9341_IOCTL_STOP_RW               0x00000400

	///	Set default config
    #define ILI9341_IOCTL_DEFAULT_CONFG			0x00000500

	///	Set page address range
    #define ILI9341_IOCTL_SET_PAGE_RANGE		0x00000600

	///	Set column address range
    #define ILI9341_IOCTL_SET_COL_RANGE			0x00000700

	///	Power on device
	#define	ILI9341_IOCTL_POWERON				0x00000800

	///	Power off device
	#define	ILI9341_IOCTL_POWEROFF				0x00000900

    /// Reset device
    #define ILI9341_IOCTL_RESET                 0x00000A00

	struct ILI9341_command{
		uint8_t		cmd;
		uint8_t		len;
		uint8_t		*param;
	};

	struct ILI9341_addrRange{
		uint16_t	start;
		uint16_t	end;
	};

	///	Initialize controller registers
	extern void ILI9341_init(void);
	///	Prepare controller to any transaction
	extern int ILI9341_open(void);
	///	Send a IOCTL command with or without buffer
	//	buffer may point to struct ILI9341_command,
	//	or struct ILI9341_{page,column}AddrRange
	extern int ILI9341_ioctl( unsigned int config , void* buffer );
	///	Write to GRAM (requires ioctl BEGNWR)
	extern int ILI9341_write( const void* buffer, unsigned int size );
	///	Write to GRAM (requires ioctl BEGNRD)
	extern int ILI9341_read( void* buffer, unsigned int size );
	///	Release controller
	extern void ILI9341_close(void);

#ifdef __cplusplus
    }
#endif

#endif
