/*!
*	@file		DriverI2C.c
*	@version	0.0
*	@author		Josepablo Cruz Baas
*	@date		08/26/2020
*	@brief		TivaC TM4C1294 specific implementation
**/
/// Include Middleware description
//  requires a compilation environment that includes
//  the directory "include/"
#include <I2C/DriverI2C.h>
#include <I2C/TivaCTM4C1294/DriverI2C_config.h>

#include <inc/hw_i2c.h>
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <inc/hw_gpio.h>

#include <driverlib/rom.h>
#include <driverlib/flash.h>
#include <driverlib/gpio.h>
#include <driverlib/i2c.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>

#define SET_RECEIVE( BASE )     HWREG(BASE + I2C_O_MSA) |= 0x01;
#define SET_TRANSMIT( BASE )     HWREG(BASE + I2C_O_MSA) &= ~0x01;

#define START_TRANSMITION               0x00
#define CONT_TRANSMITION                0x01
static volatile uint8_t transmitionState=0x00;

/// Initialize i2c buffer
void DriverI2C_init(void){
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    /* I2C7 Init */
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C7);
    /* I2C8 Init */
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C8);

    while(!ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_I2C7)||
     !ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_I2C8)     ||
     !ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)    ||
     !ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))
    {}
    /*
     * NOTE: TI-RTOS examples configure pins PD0 & PD1 for SSI2 or I2C7.  Thus,
     * a conflict occurs when the I2C & SPI drivers are used simultaneously in
     * an application.  Modify the pin mux settings in this file and resolve the
     * conflict before running your the application.
     */
    /* Configure the appropriate pins to be I2C instead of GPIO. */
//    ROM_GPIOPinConfigure(GPIO_PD0_I2C7SCL);
//    ROM_GPIOPinConfigure(GPIO_PD1_I2C7SDA);
//    ROM_GPIOPinTypeI2CSCL(GPIO_PORTD_BASE, GPIO_PIN_0);
//    ROM_GPIOPinTypeI2C(GPIO_PORTD_BASE, GPIO_PIN_1);

    /* Configure the appropriate pins to be I2C instead of GPIO. */
    ROM_GPIOPinConfigure(GPIO_PA2_I2C8SCL);
    ROM_GPIOPinConfigure(GPIO_PA3_I2C8SDA);
    ROM_GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_2);
    ROM_GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_3);
}
///	Acquire a file descriptor for i2c
int DriverI2C_open( unsigned int i2c_device ){
    if( i2c_device >= TM4C1294XL_I2CCOUNT ){
        return DriverI2C_ERROR;
    }
    ROM_I2CMasterInitExpClk( DriverI2C_devices[ i2c_device ], SYSTEM_CLOCK , false );
    ROM_I2CMasterIntDisable( DriverI2C_devices[ i2c_device ] );
    ROM_I2CMasterIntClear( DriverI2C_devices[ i2c_device ] );
    ROM_I2CMasterEnable( DriverI2C_devices[ i2c_device ] );
    ROM_I2CTxFIFOFlush( DriverI2C_devices[ i2c_device ] );
    ROM_I2CRxFIFOFlush( DriverI2C_devices[ i2c_device ] );
	return (int8_t)i2c_device;
}
int DriverI2C_ioctl( int fd , unsigned int config , void* buffer ){
    if( fd >= TM4C1294XL_I2CCOUNT){
        return DriverI2C_ERROR;
    }
    switch( DriverI2C_IOCTL_CMD_MASK & config ){
        /// Change the i2c slave address
        case DriverI2C_IOCTL_CMD_SETADDR:
            ROM_I2CMasterSlaveAddrSet( DriverI2C_devices[ fd ] , (uint8_t)(config & ~DriverI2C_IOCTL_CMD_MASK) , false );
            break;
        /// Change the i2c speed to 100Kbps
        case DriverI2C_IOCTL_CMD_SETSPEED_100K:
            ROM_I2CMasterInitExpClk( DriverI2C_devices[ fd ], SYSTEM_CLOCK , false );
            break;
        /// Change the i2c speed to 400Kbps
        case DriverI2C_IOCTL_CMD_SETSPEED_400K:
            ROM_I2CMasterInitExpClk( DriverI2C_devices[ fd ], SYSTEM_CLOCK , true );
            break;
        /// Release the i2c bus in idle state
		case DriverI2C_IOCTL_CMD_STOP_TRANSACTION:
		    transmitionState=START_TRANSMITION;
		    ROM_I2CMasterControl( DriverI2C_devices[ fd ] , I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP );///< Stop hard state machine
			break;
		/// Send a simple byte with stop bit
		case DriverI2C_IOCTL_CMD_SEND_BYTE:
		    SET_TRANSMIT( DriverI2C_devices[ fd ] );
		    /// Set data to send
		    ROM_I2CMasterDataPut( DriverI2C_devices[ fd ] , (uint8_t)(config & ~DriverI2C_IOCTL_CMD_MASK) );
		    ROM_I2CMasterIntClear( DriverI2C_devices[ fd ] );
            /// Begin transaction
		    ROM_I2CMasterControl( DriverI2C_devices[ fd ] , I2C_MASTER_CMD_SINGLE_SEND );
		        /// Wait any interrupt set
		        while( !(I2C_MASTER_INT_DATA & ROM_I2CMasterIntStatusEx( DriverI2C_devices[ fd ] , false )) ){}
                /// Check for transaction error
                if( ROM_I2CMasterErr( DriverI2C_devices[ fd ] ) ){ return DriverI2C_ERROR; }
	        break;
	    /// Receive a simple byte with stop bit
		case DriverI2C_IOCTL_CMD_RECEIVE_BYTE:
		    /// If there is not buffer available return error!
            if( !buffer ){ return DriverI2C_ERROR; }
		    SET_RECEIVE( DriverI2C_devices[ fd ] );
            ROM_I2CMasterIntClear( DriverI2C_devices[ fd ] );
            ROM_I2CMasterControl( DriverI2C_devices[ fd ] , I2C_MASTER_CMD_SINGLE_RECEIVE );
                /// Wait any interrupt set
                while( !(I2C_MASTER_INT_DATA & ROM_I2CMasterIntStatusEx( DriverI2C_devices[ fd ] , false )) ){}
                /// Check for transaction error
                if( ROM_I2CMasterErr( DriverI2C_devices[ fd ] ) ){ return DriverI2C_ERROR; }
            ((uint8_t*)buffer)[ 0 ] = (uint8_t)ROM_I2CMasterDataGet( DriverI2C_devices[ fd ] );
            break;
        /// Send a single byte and read a single byte
		case DriverI2C_IOCTL_CMD_READ_REG:
            /// If there is not buffer available return error!
            if( !buffer ){ return DriverI2C_ERROR; }

            ///
            //  Send a single byte without sending a stop bit
            //

            SET_TRANSMIT( DriverI2C_devices[ fd ] );
            ROM_I2CMasterDataPut( DriverI2C_devices[ fd ] , (uint8_t)(config & ~DriverI2C_IOCTL_CMD_MASK) );
            ROM_I2CMasterIntClear( DriverI2C_devices[ fd ] );
            ROM_I2CMasterControl( DriverI2C_devices[ fd ] , I2C_MASTER_CMD_SINGLE_SEND );
                /// Wait any interrupt set
                while( !(I2C_MASTER_INT_DATA & ROM_I2CMasterIntStatusEx( DriverI2C_devices[ fd ] , false )) ){}
                /// Check for transaction error
                if( ROM_I2CMasterErr( DriverI2C_devices[ fd ] ) ){///< Check for transaction error
                    ROM_I2CMasterControl( DriverI2C_devices[ fd ] , I2C_MASTER_CMD_BURST_SEND_ERROR_STOP );
                    return DriverI2C_ERROR;
                }

            ///
            //  Receive a single byte and stop transaction
            //

            SET_RECEIVE( DriverI2C_devices[ fd ] );
            ROM_I2CMasterIntClear( DriverI2C_devices[ fd ] );
            ROM_I2CMasterControl( DriverI2C_devices[ fd ] , I2C_MASTER_CMD_SINGLE_RECEIVE );
                /// Wait any interrupt set
                while( !(I2C_MASTER_INT_DATA & ROM_I2CMasterIntStatusEx( DriverI2C_devices[ fd ] , false )) ){}
                if( ROM_I2CMasterErr( DriverI2C_devices[ fd ] ) ){
                    return DriverI2C_ERROR;
                }
            ((uint8_t*)buffer)[ 0 ] = (uint8_t)ROM_I2CMasterDataGet( DriverI2C_devices[ fd ] );
            break;
    }
    return 0;
}
///	Release a file descriptor for i2c
void DriverI2C_close( int fd ){
    if( fd >= TM4C1294XL_I2CCOUNT ){
        return;///< Nothing to do
    }
    ROM_I2CMasterDisable( DriverI2C_devices[ fd ] );
}
/**
*	Begins a transaction with a control byte sending the i2c address first,
*	followed by the buffer content.
*	In case of any transaction error, stops the transaction returning the bus
*	to the idle state.
*	In case of success transaction, the line does not return to the idle state.
*
*	@return int32_t total of bytes sended, in case of error returns DriverI2C_ERROR
 */
int DriverI2C_write( int fd, const void* buffer, unsigned int size ){
    uint32_t i2cPeriphBase;
    uint32_t byteCounter;
	if( !buffer || !size ){///< No buffer , size too small
		return DriverI2C_ERROR;
	}
    if( fd >= TM4C1294XL_I2CCOUNT ){
        return DriverI2C_ERROR;
    }

    /// Set i2c hardware for transaction
    i2cPeriphBase = DriverI2C_devices[ fd ];
    SET_TRANSMIT( i2cPeriphBase );

    /// Send first byte
    ROM_I2CMasterDataPut( i2cPeriphBase , ((uint8_t*)buffer)[ 0 ] );
    ROM_I2CMasterIntClear( DriverI2C_devices[ fd ] );

    /// Begin a multi transaction
    if( transmitionState == START_TRANSMITION ){
        transmitionState = CONT_TRANSMITION;
        ROM_I2CMasterControl( i2cPeriphBase , I2C_MASTER_CMD_BURST_SEND_START );
    }else{
        ROM_I2CMasterControl( i2cPeriphBase , I2C_MASTER_CMD_BURST_SEND_CONT );
    }
    /// Wait for transaction to finish
    while( !(I2C_MASTER_INT_DATA & ROM_I2CMasterIntStatusEx( DriverI2C_devices[ fd ] , false )) ){}
    if( ROM_I2CMasterErr( i2cPeriphBase ) ){
        transmitionState = START_TRANSMITION;
        ROM_I2CMasterControl( i2cPeriphBase , I2C_MASTER_CMD_BURST_SEND_ERROR_STOP );///< Stop hard state machine
        return (int32_t)(0);
    }

	/// For every byte to send do
	for( byteCounter=1; byteCounter < size ; byteCounter++ ){
	    ROM_I2CMasterDataPut( i2cPeriphBase , ((uint8_t*)buffer)[ byteCounter ] );
        ROM_I2CMasterIntClear( DriverI2C_devices[ fd ] );
	    ROM_I2CMasterControl( i2cPeriphBase , I2C_MASTER_CMD_BURST_SEND_CONT );
        /// Wait for transaction to finish
        while( !(I2C_MASTER_INT_DATA & ROM_I2CMasterIntStatusEx( DriverI2C_devices[ fd ] , false )) ){}
	    if( ROM_I2CMasterErr( i2cPeriphBase ) ){
            transmitionState = START_TRANSMITION;
	        ROM_I2CMasterControl( i2cPeriphBase , I2C_MASTER_CMD_BURST_SEND_ERROR_STOP );///< Stop hard state machine
	        return (int32_t)(byteCounter-1);
	    }
	}
    return (int32_t)(size);
}
/**
*	Begins a transaction with a control byte sending the i2c address first with
*	the reading flag active. The rest of the transaction are the bytes from slave.
*	In case of any transaction error, stops the transaction returning the bus
*	to the idle state.
*	In case of success transaction, the line does not return to the idle state.
*
*	@return int32_t total of bytes received, in case of error returns DriverI2C_ERROR
 */
int DriverI2C_read( int fd, void* buffer, unsigned int size ){
    uint32_t i2cPeriphBase;
    uint32_t byteCounter;
    if( !buffer || !size  ){///< No buffer , size too small
        return DriverI2C_ERROR;
    }
    if( fd >= TM4C1294XL_I2CCOUNT ){
        return DriverI2C_ERROR;
    }

    /// Set i2c hardware for reception
    i2cPeriphBase = DriverI2C_devices[ fd ];
    SET_RECEIVE( i2cPeriphBase );
    ROM_I2CMasterIntClear( DriverI2C_devices[ fd ] );

    /// Begin a multi transaction
    if( transmitionState == START_TRANSMITION ){
        transmitionState = CONT_TRANSMITION;
        ROM_I2CMasterControl( i2cPeriphBase , I2C_MASTER_CMD_BURST_RECEIVE_START );
    }else{
        ROM_I2CMasterControl( i2cPeriphBase , I2C_MASTER_CMD_BURST_RECEIVE_CONT );
    }
        /// Wait for transaction to finish
        while( !(I2C_MASTER_INT_DATA & ROM_I2CMasterIntStatusEx( DriverI2C_devices[ fd ] , false )) ){}
        if( ROM_I2CMasterErr( i2cPeriphBase ) ){
            transmitionState = START_TRANSMITION;
            ROM_I2CMasterControl( i2cPeriphBase , I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP );///< Stop hard state machine
            return 0;
        }
    ((uint8_t*)buffer)[ 0 ] = (uint8_t)ROM_I2CMasterDataGet( i2cPeriphBase );

    /// For a burst send operation do
    for( byteCounter=1; byteCounter < size ; byteCounter++ ){
        ROM_I2CMasterIntClear( DriverI2C_devices[ fd ] );
        ROM_I2CMasterControl( i2cPeriphBase , I2C_MASTER_CMD_BURST_RECEIVE_CONT );
        /// Wait for transaction to finish
        while( !(I2C_MASTER_INT_DATA & ROM_I2CMasterIntStatusEx( DriverI2C_devices[ fd ] , false )) ){}
        if( ROM_I2CMasterErr( i2cPeriphBase ) ){
            transmitionState = START_TRANSMITION;
            ROM_I2CMasterControl( i2cPeriphBase , I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP );///< Stop hard state machine
            return (int32_t)(byteCounter-1);
        }
        ((uint8_t*)buffer)[ byteCounter ] = (uint8_t)ROM_I2CMasterDataGet( i2cPeriphBase );
    }

    return (int32_t)(size);
}

/*
 *  https://e2e.ti.com/support/microcontrollers/other/f/908/t/343532?I2C-Busy-and-Error-Flags-on-TM4C1292
 *  https://jspicer.net/2018/07/27/solution-for-i2c-busy-status-latency/
 *
 * Here says that there's a delay between busy flag to be set before any polling.
 * Also say that any final transaction clears the Err Flags so is necessary to check
 * the interrupts in order to validate the Err Flags instead of the Err Flags.
 * */
