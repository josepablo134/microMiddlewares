/// Include Middleware description
//  requires a compilation environment that includes
//  the directory "include/"
#include <SPI/DriverSPI.h>
#include <SPI/TivaCTM4C1294/DriverSPI_config.h>

#include <inc/hw_ssi.h>
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <inc/hw_gpio.h>

#include <driverlib/rom.h>
#include <driverlib/flash.h>
#include <driverlib/gpio.h>
#include <driverlib/ssi.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/uart.h>

#define int_DriverSPI_ASSERT_FD( FD ){  if( FD >= TM4C1294XL_SPICOUNT ){ return -1; } }
#define void_DriverSPI_ASSERT_FD( FD ){  if( FD >= TM4C1294XL_SPICOUNT ){ return; } }

void DriverSPI_init(void){
    /// Clear all SPI devices flags
    uint8_t device;
    for( device = 0; device < TM4C1294XL_SPICOUNT ; device ++ ){
        DriverSPI_flags[ device ] = 0x00;
    }
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);
    /* SSI2 */
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);
    /* SSI3 */
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI3);

    while(!ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_SSI2)||
     !ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_SSI3)     ||
     !ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)    ||
     !ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOQ))
    {}
    /* SSI2 */
    /*
     * NOTE: TI-RTOS examples configure pins PD0 & PD1 for SSI2 or I2C7.  Thus,
     * a conflict occurs when the I2C & SPI drivers are used simultaneously in
     * an application.  Modify the pin mux settings in this file and resolve the
     * conflict before running your the application.
     */

    ROM_GPIOPinConfigure(GPIO_PD3_SSI2CLK);
    ROM_GPIOPinConfigure(GPIO_PD2_SSI2FSS);
    ROM_GPIOPinConfigure(GPIO_PD1_SSI2XDAT0);//MOSI
    ROM_GPIOPinConfigure(GPIO_PD0_SSI2XDAT1);//MISO

    ROM_GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 |
                                    GPIO_PIN_2 | GPIO_PIN_3);

    ROM_GPIOPinConfigure(GPIO_PQ0_SSI3CLK);
    ROM_GPIOPinConfigure(GPIO_PQ1_SSI3FSS);
    ROM_GPIOPinConfigure(GPIO_PQ2_SSI3XDAT0);//MOSI
    ROM_GPIOPinConfigure(GPIO_PQ3_SSI3XDAT1);//MISO

    ROM_GPIOPinTypeSSI(GPIO_PORTQ_BASE, GPIO_PIN_0 | GPIO_PIN_1 |
                                    GPIO_PIN_2 | GPIO_PIN_3);
}

int DriverSPI_open( unsigned int spi_device){
    int_DriverSPI_ASSERT_FD( spi_device );
    uint32_t    periph_base = DriverSPI_devices[ spi_device ];

    ROM_SSIDisable( periph_base );
    ROM_SSIConfigSetExpClk( periph_base , SYSTEM_CLOCK ,
                        SSI_FRF_MOTO_MODE_0 , SSI_MODE_MASTER ,
                        DriverSPI_DEFAULT_BITRATE , 8 );
    ROM_SSIAdvModeSet( periph_base, SSI_ADV_MODE_READ_WRITE );
    ROM_SSIAdvFrameHoldEnable( periph_base );
    ROM_SSIIntClear( periph_base , 0x7F );
    ROM_SSIIntDisable( periph_base , 0x7F );
    ROM_SSIEnable( periph_base );
    return spi_device;
}


/* *
 *  Notes:
 *  SSI_FRF_MOTO_MODE0 => Polarity (0) Phase (0) = 0x00
 *  SSI_FRF_MOTO_MODE1 => Polarity (0) Phase (1) = 0x02
 *  SSI_FRF_MOTO_MODE2 => Polarity (1) Phase (0) = 0x01
 *  SSI_FRF_MOTO_MODE3 => Polarity (1) Phase (1) = 0x03
 */
int DriverSPI_ioctl( int fd , unsigned int config , void* buffer ){
    int_DriverSPI_ASSERT_FD( fd );
    /// Assuming already open device
    uint32_t    periph_base = DriverSPI_devices[ fd ];

    switch( config & DriverSPI_IOCTL_CMD_MASK ){
        case DriverSPI_IOCTL_CONF_MODE:
        {
            if(!buffer){ return 0x00;}
            struct DriverSPI_mode* mode = (struct DriverSPI_mode*)buffer;

            uint8_t pol_phase_mode = (0x01 & mode->phase) << 1;
            pol_phase_mode |= (0x01 & mode->polarity);

            ROM_SSIDisable( periph_base );
            ROM_SSIConfigSetExpClk( periph_base , SYSTEM_CLOCK ,
                                    pol_phase_mode , SSI_MODE_MASTER ,
                                    mode->bitrate , 8 );
            ROM_SSIAdvModeSet( periph_base, SSI_ADV_MODE_READ_WRITE );
            ROM_SSIAdvFrameHoldEnable( periph_base );
            ROM_SSIIntClear( periph_base , 0x7F );
            ROM_SSIIntDisable( periph_base , 0x7F );
            ROM_SSIEnable( periph_base );
            return 0;
        }
        case DriverSPI_IOCTL_CONF_FRAME_MODE:
        {
            DriverSPI_flags[ fd ] &= (uint8_t) ~( DriverSPI_MODE_MASK );/// Clear actual mode
            if( DriverSPI_MODE_FRAME_PARTITION == ( config & 0xFF ) ){
                DriverSPI_flags[ fd ] |= DriverSPI_MODE_FRAME_PARTITION;/// Set frame partition mode
            }/// Normal mode is equal to clear flag.
            return 0;
        }
    }
    return -1;
}

void DriverSPI_close( int fd ){
    void_DriverSPI_ASSERT_FD( fd );
}

int DriverSPI_write( int fd, const void* buffer, unsigned int size){
    int_DriverSPI_ASSERT_FD( fd );
    if( !buffer || !size ){
        return -1;
    }
    /// Assuming already open device
    uint32_t    periph_base = DriverSPI_devices[ fd ];
    uint8_t*    pvBuffer = (uint8_t*)buffer;
    unsigned int counter;
    uint32_t dummy;

    if(  DriverSPI_MODE_FRAME_NORMAL == ( DriverSPI_flags[ fd ] & DriverSPI_MODE_MASK )  ){
        size--;// Size is atleast 1, this could lead to size = 0 in the worse case.
        for( counter=0; counter < size; counter++ ){
            SSIDataPut( periph_base , pvBuffer[counter] );/// TivaWare specifies that this is a blocking mode
        }
        /* *
         * Send this byte as the end of the frame,
         * this leads to releasing the CS pin
         * */
        SSIAdvDataPutFrameEnd( periph_base , pvBuffer[counter] );
        /// Flush RX FIFO
        while (SSIBusy(periph_base)){}
    }else{
        /* *
         * Send data without releasing the CS pin,
         * that will be done in future transactions
         *  */
        for( counter=0; counter < size; counter++ ){
            SSIDataPut( periph_base , pvBuffer[counter] );/// TivaWare specifies that this is a blocking mode
        }
    }
    while( SSIDataGetNonBlocking( periph_base , &dummy) ){}/// Flush FIFO
    return counter;
}

int DriverSPI_read( int fd, void* buffer, unsigned int size){
    int_DriverSPI_ASSERT_FD( fd );
    if( !buffer || !size ){
        return -1;
    }
    /// Assuming already open device
    uint32_t    periph_base = DriverSPI_devices[ fd ];
    uint8_t*    pvBuffer = (uint8_t*)buffer;
    uint32_t    temp;
    unsigned int counter;

    if(  DriverSPI_MODE_FRAME_NORMAL == ( DriverSPI_flags[ fd ] & DriverSPI_MODE_MASK )  ){
        size--;// Size is atleast 1, this could lead to size = 0 in the worse case.
        for( counter=0; counter < size; counter++ ){
            SSIDataPut( periph_base , pvBuffer[counter] );
            SSIDataGet( periph_base , &temp );
            pvBuffer[counter] = (uint8_t)(0xFF & temp);
            temp = 0x00;
        }
        /* *
         * Send this byte as the end of the frame,
         * this leads to releasing the CS pin.
         * Then read the last byte at the RX buffer.
         * */
        SSIAdvDataPutFrameEnd( periph_base , pvBuffer[counter] );
        while (SSIBusy(periph_base)){}
        SSIDataGet( periph_base , &temp );
        pvBuffer[counter] = (uint8_t)(0xFF & temp);
    }else{
        /* *
         * Send/Receive data without releasing the CS pin,
         * that will be done in future transactions
         *  */
        for( counter=0; counter < size; counter++ ){
            SSIDataPut( periph_base , pvBuffer[counter] );/// TivaWare specifies that this is a blocking mode
            SSIDataGet( periph_base , &temp );
            pvBuffer[counter] = (0xFF & temp);
            temp = 0x00;
        }
    }
    return counter;
}
