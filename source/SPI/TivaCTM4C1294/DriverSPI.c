/// Include Middleware description
//  requires a compilation environment that includes
//  the directory "include/"
#include <SPI/DriverSPI.h>
#include <SPI/DriverSPI_config.h>

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

void DriverSPI_init(void){
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
    ROM_GPIOPinConfigure(GPIO_PD1_SSI2XDAT0);
    ROM_GPIOPinConfigure(GPIO_PD0_SSI2XDAT1);

    ROM_GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 |
                                    GPIO_PIN_2 | GPIO_PIN_3);

    ROM_GPIOPinConfigure(GPIO_PQ0_SSI3CLK);
    ROM_GPIOPinConfigure(GPIO_PQ1_SSI3FSS);
    ROM_GPIOPinConfigure(GPIO_PQ2_SSI3XDAT0);
    ROM_GPIOPinConfigure(GPIO_PQ3_SSI3XDAT1);

    ROM_GPIOPinTypeSSI(GPIO_PORTQ_BASE, GPIO_PIN_0 | GPIO_PIN_1 |
                                    GPIO_PIN_2 | GPIO_PIN_3);
}
int DriverSPI_open( unsigned int i2c_device){
    return -1;
}

int DriverSPI_ioctl( int fd , unsigned int config , void* buffer ){
    return -1;
}

void DriverSPI_close( int fd ){

}

int DriverSPI_write( int fd, const void* buffer, unsigned int size){
    return -1;
}

int DriverSPI_read( int fd, void* buffer, unsigned int size){
    return -1;
}
