/// C std library
#include <stdbool.h>
#include <stdint.h>

/// Cross dependencies
#include <BUS8080/Driver8080.h>
#include <DEBUG.h>

/// Driver definition
#include <ILI9341/ILI9341.h>
#include <ILI9341/ILI9341_regmap.h>

///	Initialize controller registers
void ILI9341_init(void){
    Driver8080_init();
}

///	Prepare controller to any transaction
int ILI9341_open(void){
    return Driver8080_open();
}

///	Send a IOCTL command with or without buffer
//	buffer may point to struct ILI9341_command,
//	or struct ILI9341_{page,column}AddrRange
int ILI9341_ioctl( unsigned int config , void* buffer ){
	switch( config & Driver8080_IOCTL_CMD_MASK ){
		case ILI9341_IOCTL_SEND_CMD:
		{
		    if( !buffer ){ return -1; }
			struct ILI9341_command	*cmd;
			cmd = (struct ILI9341_command*) buffer;

			/// Ensuring IDLE BUS
			Driver8080_ioctl( Driver8080_IOCTL_CMD_RELEASE, 0x00);

			Driver8080_ioctl( Driver8080_IOCTL_CMD_BEGIN_WR | (cmd->cmd), 0x00);
			Driver8080_write( cmd->param , cmd->len );

			/// Releasing bus
            Driver8080_ioctl( Driver8080_IOCTL_CMD_RELEASE, 0x00);
			return 0;
		}
		case ILI9341_IOCTL_BEGIN_WR:
		{
            /// Ensuring IDLE BUS
            Driver8080_ioctl( Driver8080_IOCTL_CMD_RELEASE, 0x00);

            /// Sending Address and keep bus active
			return Driver8080_ioctl( Driver8080_IOCTL_CMD_BEGIN_WR | ( ILI9341_IOCTL_DATA_MASK & config), 0x00);
		}
		case ILI9341_IOCTL_BEGIN_RD:
		{
            /// Ensuring IDLE BUS
            Driver8080_ioctl( Driver8080_IOCTL_CMD_RELEASE, 0x00);

            /// Sending Address and keep bus active
			return Driver8080_ioctl( Driver8080_IOCTL_CMD_BEGIN_RD | ( ILI9341_IOCTL_DATA_MASK & config) , 0x00);
		}
		case ILI9341_IOCTL_STOP_RW:
		{
		    /// Force BUS release
			return Driver8080_ioctl(Driver8080_IOCTL_CMD_RELEASE, 0x00);
		}
    	case ILI9341_IOCTL_DEFAULT_CONFG:
    	{
			return -1;
		}
    	case ILI9341_IOCTL_SET_PAGE_RANGE:
		{
            if( !buffer ){ return -1; }
            /// Ensuring IDLE BUS
            Driver8080_ioctl( Driver8080_IOCTL_CMD_RELEASE, 0x00);

            struct ILI9341_pageAddrRange  *range;
            range = (struct ILI9341_pageAddrRange*) buffer;

            /// Sending SET PAGE command
            Driver8080_ioctl( Driver8080_IOCTL_CMD_BEGIN_WR | ILI9341_PAG_ADDR_SET, 0x00);
            Driver8080_write( &range->startPage , 2 );
            Driver8080_write( &range->endPage , 2 );

            /// Releasing bus
            Driver8080_ioctl( Driver8080_IOCTL_CMD_RELEASE, 0x00);
            return 0;
		}
    	case ILI9341_IOCTL_SET_COL_RANGE:
		{
            if( !buffer ){ return -1; }
            /// Ensuring IDLE BUS
            Driver8080_ioctl( Driver8080_IOCTL_CMD_RELEASE, 0x00);

            struct ILI9341_columnAddrRange  *range;
            range = (struct ILI9341_columnAddrRange*) buffer;

            /// Sending SET START PAGE command
            Driver8080_ioctl( Driver8080_IOCTL_CMD_BEGIN_WR | ILI9341_COL_ADDR_SET, 0x00);
            Driver8080_write( &range->startColumn , 2 );
            Driver8080_write( &range->endColumn , 2 );

            /// Releasing bus
            Driver8080_ioctl( Driver8080_IOCTL_CMD_RELEASE, 0x00);
            return 0;
		}
		case ILI9341_IOCTL_POWERON:
		{
            /// Ensuring IDLE BUS
            Driver8080_ioctl( Driver8080_IOCTL_CMD_RELEASE, 0x00);
		    Driver8080_ioctl( Driver8080_IOCTL_CMD_BEGIN_WR | ILI9341_DISPLAY_ON , 0x00 );
            /// Releasing bus
            return Driver8080_ioctl( Driver8080_IOCTL_CMD_RELEASE, 0x00);
		}
		case ILI9341_IOCTL_POWEROFF:
		{
            /// Ensuring IDLE BUS
            Driver8080_ioctl( Driver8080_IOCTL_CMD_RELEASE, 0x00);
		    Driver8080_ioctl( Driver8080_IOCTL_CMD_BEGIN_WR | ILI9341_DISPLAY_OFF , 0x00 );
            /// Releasing bus
            return Driver8080_ioctl( Driver8080_IOCTL_CMD_RELEASE, 0x00);
		}
		case ILI9341_IOCTL_RESET:
		{
		    return Driver8080_ioctl( Driver8080_IOCTL_CMD_RST , 0x00);
		}
	}
    return -1;
}

///	Write to GRAM (requires ioctl BEGNWR)
int ILI9341_write( const void* buffer, unsigned int size ){
	return Driver8080_write( buffer , size );
}

///	Write to GRAM (requires ioctl BEGNRD)
int ILI9341_read( void* buffer, unsigned int size ){
	return Driver8080_read( buffer , size );
}

///	Release controller
void ILI9341_close(void){

}
