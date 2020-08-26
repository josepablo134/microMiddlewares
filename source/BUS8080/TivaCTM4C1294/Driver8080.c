#include <BUS8080/Driver8080.h>
/// Dependencias locales
#include <stdbool.h>
#include <stdint.h>

#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <inc/hw_gpio.h>

#include <driverlib/rom.h>
#include <driverlib/flash.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>

    /// This is not a IOCTL command
    #define Driver8080_IOCTL_CMD_MASK            	0xFFFFFF00

	///	This is not a IOCTL command
    #define Driver8080_IOCTL_DATA_MASK            	0x000000FF

#define	DATA_BUS_PERIPH		SYSCTL_PERIPH_GPIOK
#define DATA_BUS_BASE		GPIO_PORTK_BASE
#define	DATA_BUS_MASK		0xFF

#define CTL_BUS_PERIPH		SYSCTL_PERIPH_GPIOE
#define CTL_BUS_BASE		GPIO_PORTE_BASE
#define CTL_BUS_MASK		0x1F
#define	CTL_BUS_CS			0x01
#define	CTL_BUS_WR			0x02
#define	CTL_BUS_RD			0x04
#define	CTL_BUS_DC			0x08
#define CTL_BUS_RST			0x10

/// Disable RD and strobe WR
#define	STROBE_WR_INIT(){ROM_GPIOPinWrite( CTL_BUS_BASE,CTL_BUS_RD|CTL_BUS_WR, CTL_BUS_RD );}
#define	STROBE_WR()		{ROM_GPIOPinWrite( CTL_BUS_BASE,CTL_BUS_WR,CTL_BUS_WR );}

/// Disable WR and strobe RD
#define	STROBE_RD_INIT(){ROM_GPIOPinWrite( CTL_BUS_BASE,CTL_BUS_RD|CTL_BUS_WR, CTL_BUS_WR );}
#define	STROBE_RD()		{ROM_GPIOPinWrite( CTL_BUS_BASE,CTL_BUS_RD,CTL_BUS_RD );}

#define STROBE_RST_INIT(){ROM_GPIOPinWrite( CTL_BUS_BASE,CTL_BUS_RST,0x00);}
#define STROBE_RST(){ROM_GPIOPinWrite( CTL_BUS_BASE,CTL_BUS_RST,CTL_BUS_RST);}

/// Enable bus setting CS
#define ENABLE_BUS()	{ROM_GPIOPinWrite( CTL_BUS_BASE , CTL_BUS_CS , 0x00 );}

#define DISABLE_BUS()	{ROM_GPIOPinWrite( CTL_BUS_BASE , CTL_BUS_MASK , CTL_BUS_MASK );}

/// Set bus for data writing
#define DATA_MODE()		{ROM_GPIOPinWrite( CTL_BUS_BASE , CTL_BUS_DC , CTL_BUS_DC );}

/// Set bus for cmd writing
#define CMD_MODE()		{ROM_GPIOPinWrite( CTL_BUS_BASE , CTL_BUS_DC , 0x00 );}

/// Set data lines as input
#define	SET_BUS_INPUT()		{ROM_GPIOPinTypeGPIOInput( DATA_BUS_BASE , DATA_BUS_MASK );}

/// Set data lines as output
#define	SET_BUS_OUTPUT()	{ROM_GPIOPinTypeGPIOOutput( DATA_BUS_BASE , DATA_BUS_MASK );\
	GPIOPadConfigSet( DATA_BUS_BASE , DATA_BUS_MASK , GPIO_STRENGTH_12MA , GPIO_PIN_TYPE_STD );}

#define SET_DATA( DATA )	{ROM_GPIOPinWrite( DATA_BUS_BASE , DATA_BUS_MASK, DATA);}

#define READ_DATA()         (uint8_t)ROM_GPIOPinRead( DATA_BUS_BASE , DATA_BUS_MASK)

void Driver8080_init(void){
    ROM_SysCtlPeripheralEnable( DATA_BUS_PERIPH );
    ROM_SysCtlPeripheralEnable( CTL_BUS_PERIPH );
    while( !ROM_SysCtlPeripheralReady( DATA_BUS_PERIPH ) ||
	!ROM_SysCtlPeripheralReady( CTL_BUS_PERIPH ) )
	{}
}

int Driver8080_open( void ){
    ROM_GPIOPinTypeGPIOOutput( DATA_BUS_BASE , DATA_BUS_MASK );
    ROM_GPIOPinTypeGPIOOutput( CTL_BUS_BASE , CTL_BUS_MASK );
	GPIOPadConfigSet( DATA_BUS_BASE , DATA_BUS_MASK , GPIO_STRENGTH_12MA , GPIO_PIN_TYPE_STD );
    GPIOPadConfigSet( CTL_BUS_BASE , CTL_BUS_MASK , GPIO_STRENGTH_12MA , GPIO_PIN_TYPE_STD );

    DISABLE_BUS();
	return 0;
}

int Driver8080_ioctl( unsigned int config , void* buffer ){
	switch( config & Driver8080_IOCTL_CMD_MASK ){
		case Driver8080_IOCTL_CMD_RELEASE:
		{
			SET_BUS_OUTPUT();
			SET_DATA( 0x00 );
			DISABLE_BUS();
			return 0;
		}
		case Driver8080_IOCTL_CMD_BEGIN_WR:
		{
			///	Sending the address
			ENABLE_BUS();
			SET_BUS_OUTPUT();
            CMD_MODE();
			STROBE_WR_INIT();
				SET_DATA( config & Driver8080_IOCTL_DATA_MASK );
			STROBE_WR();
			return 0;
		}
		case Driver8080_IOCTL_CMD_BEGIN_RD:
		{
			///	Sending the address
			ENABLE_BUS();
			SET_BUS_OUTPUT();
            CMD_MODE();
			STROBE_WR_INIT();
				SET_DATA( config & Driver8080_IOCTL_DATA_MASK );
			STROBE_WR();

			///	Reading invalid byte
			SET_BUS_INPUT();
			DATA_MODE();
			STROBE_RD_INIT();
			STROBE_RD();
			return 0;
		}
		case Driver8080_IOCTL_CMD_BEGIN_WR_ARR:
		{
			if( !buffer ){return -1;}
			uint8_t		counter;
			ENABLE_BUS();
			SET_BUS_OUTPUT();
			CMD_MODE();
			for( counter=0; counter < config & Driver8080_IOCTL_DATA_MASK; counter++){
				STROBE_WR_INIT();
				SET_DATA( ((uint8_t*)buffer)[counter] );
				STROBE_WR();
			}
			return 0;
		}
		case Driver8080_IOCTL_CMD_BEGIN_RD_ARR:
		{
			if( !buffer ){return -1;}
			uint8_t		counter;
			ENABLE_BUS();
			SET_BUS_OUTPUT();
			CMD_MODE();
			for( counter=0; counter < config & Driver8080_IOCTL_DATA_MASK; counter++){
				STROBE_WR_INIT();
				SET_DATA( ((uint8_t*)buffer)[counter] );
				STROBE_WR();
			}
			///	Reading invalid byte
			SET_BUS_INPUT();
			DATA_MODE();
			STROBE_RD_INIT();
			STROBE_RD();
			return 0;
		}
		case Driver8080_IOCTL_CMD_RST:
		{
			DISABLE_BUS();
			STROBE_RST_INIT();
			STROBE_RST();
			return 0;
		}
	}
	return 0;
}

void Driver8080_close( void ){
}

int Driver8080_write( const void* buffer, unsigned int size ){
	///	Assuming bus enabled
	if( !buffer ){return -1;}
	unsigned int	counter;
	SET_BUS_OUTPUT();
	DATA_MODE();
	for( counter=0; counter < size; counter++){
		STROBE_WR_INIT();
		SET_DATA( ((uint8_t*)buffer)[counter] );
		STROBE_WR();
	}
	return counter;
}

int Driver8080_read( void* buffer, unsigned int size ){
	///	Assuming bus enabled
	if( !buffer ){return -1;}
	unsigned int	counter;
	SET_BUS_INPUT();
	DATA_MODE();
	for( counter=0; counter < size; counter++){
		STROBE_RD_INIT();
		((uint8_t*)buffer)[counter] = READ_DATA();
		STROBE_RD();
	}
	return counter;
}
