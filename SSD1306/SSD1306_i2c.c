/// Function prototypes
#include "SSD1306.h"
#include "SSD1306_i2c.h"
/// Specific configuration
#include "SSD1306_i2c_config.h"
#include "SSD1306_i2c_regmap.h"

/// Middleware
#include <I2C/DriverI2C.h>
#include <I2C/DriverI2C_config.h>

/// Private attributes
static int8_t   i2c_fd = (-1);

void SSD1306_init(){
    DriverI2C_init();
}

int SSD1306_open(){
    i2c_fd = DriverI2C_open( SSD1306_I2C );
    if( i2c_fd < 0 ){ return -1; }
    DriverI2C_ioctl( i2c_fd , SSD1306_I2C_SPEED , 0x00 );
    DriverI2C_ioctl( i2c_fd , DriverI2C_IOCTL_CMD_SETADDR | SSD1306_I2C_SLAVE_ADDR , 0x00 );
    return 0;
}

int SSD1306_write( void *buffer , uint32_t size ){
    if( !buffer || !size ){
        return -1;
    }
    int totalWritted;
    uint8_t commandBuffer = OLED_SEND_DATA;
    if( DriverI2C_write( i2c_fd , &commandBuffer , 1 ) < 1 ){
        return -1;
    }
    totalWritted = (int) DriverI2C_write( i2c_fd , buffer , size );
    DriverI2C_ioctl(i2c_fd , DriverI2C_IOCTL_CMD_STOP_TRANSACTION , 0x00 );
    return totalWritted;
}

int SSD1306_ioctl(uint32_t config , void* buffer){
    switch( config & SSD1306_IOCTL_CMD_MASK ){
        case SSD1306_IOCTL_POWER_SET_OFF:
        {
            return SSD1306_powerOff();
        }
        case SSD1306_IOCTL_POWER_SET_ON:
        {
            return SSD1306_powerOn();
        }
        case SSD1306_IOCTL_DEFAULT_CONFG:
        {
            return SSD1306_defaultConfig();
        }
        case SSD1306_IOCTL_SENDCMD_BYTE:
        {
            return SSD1306_writeCommand( (uint8_t)(SSD1306_IOCTL_CMD_DATA & config) );
        }
        case SSD1306_IOCTL_SENDCMD_ARRY:
        {    if( !buffer ){ return SSD1306_ERROR; }
            uint8_t size = (uint8_t)(SSD1306_IOCTL_CMD_DATA & config);
            if( SSD1306_writeCommandList( buffer , size ) < size ){
                return -1;
            }
        }
        case SSD1306_IOCTL_CLC:
        {
            return SSD1306_clc();
        }
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
/// Private functions
///////////////////////////////////////////////////////////////////////////////////////////
int SSD1306_defaultConfig(void){
    uint8_t     confBuff[]={
        OLED_DISPLAYOFF,          // Screen Off
        OLED_SETDISPLAYCLOCKDIV, 0x90,
        OLED_SETMULTIPLEX, 63,
        OLED_SETDISPLAYOFFSET, 0x00,
        OLED_SETSTARTLINE_ZERO,
        OLED_SEGREMAPINV,
        OLED_COMSCANDEC,
        OLED_SETCOMPINS, 0x12,
        OLED_SETCONTRAST, 0x07,
        OLED_SETPRECHARGE, 0x22,
        OLED_SETVCOMDESELECT, 0x30,
        OLED_DISPLAYALLONRESUME,
        OLED_DISPLAYNORMAL,
        OLED_CHARGEPUMP, OLED_CHARGEPUMP_ON,
        OLED_SETMEMORYMODE, OLED_SETMEMORYMODE_HORIZONTAL,
        OLED_SETPAGEADDR, 0 , 7,
        OLED_SETCOLUMNADDR, 0 , 127,
        OLED_DISPLAYON
    };
    return SSD1306_writeCommandList( confBuff , sizeof(confBuff) );
}

int SSD1306_clc( void ){
    uint8_t displayBuffer[16];
    uint32_t counter;
    for(counter=0;counter < sizeof(displayBuffer) ; counter++){
        displayBuffer[counter] = 0x00;
    }
    SSD1306_setColumn( 0 , 127 );///< Assuming 128 pixels width
    SSD1306_setPage( 0 , 7 );///< Assuming 64 pixels height

    for( counter=0; counter < 64 ; counter++ ){
        if( SSD1306_write( displayBuffer , sizeof( displayBuffer ) ) < sizeof( displayBuffer ) ){
            return -1;
        }
    }

    return 0;
}

int SSD1306_setPage( uint8_t pageInit , uint8_t pageEnd ){
    uint8_t commandBuffer[3];
    commandBuffer[0] = OLED_SETPAGEADDR;
    commandBuffer[1] = pageInit;
    commandBuffer[2] = pageEnd;
    return SSD1306_writeCommandList( commandBuffer , sizeof(commandBuffer) );
}

int SSD1306_setColumn( uint8_t columnInit , uint8_t columnEnd ){
    uint8_t commandBuffer[3];
    commandBuffer[0] = OLED_SETCOLUMNADDR;
    commandBuffer[1] = columnInit;
    commandBuffer[2] = columnEnd;
    return SSD1306_writeCommandList( commandBuffer , sizeof(commandBuffer) );
}

int SSD1306_powerOn(void){
    return SSD1306_writeCommand(OLED_DISPLAYON);
}

int SSD1306_powerOff(void){
    return SSD1306_writeCommand(OLED_DISPLAYOFF);
}

int SSD1306_writeCommand( uint8_t command ){
    uint8_t     commandBuffer[2]={OLED_SEND_COMMAND};
    commandBuffer[1] = command;
    if( DriverI2C_write( i2c_fd , commandBuffer , 2 ) < 2 ){
        return -1;
    }
    DriverI2C_ioctl(i2c_fd , DriverI2C_IOCTL_CMD_STOP_TRANSACTION , 0x00 );
    return 0;
}

int SSD1306_writeCommandList( void *buffer , uint8_t size ){
    if( !buffer || !size ){
        return -1;
    }
    uint8_t commandBuffer = OLED_SEND_COMMAND;
    if( DriverI2C_write( i2c_fd , &commandBuffer , 1 ) < 1 ){
        return -1;
    }
    if( DriverI2C_write( i2c_fd , buffer , size ) < size ){
        return -1;
    }
    DriverI2C_ioctl(i2c_fd , DriverI2C_IOCTL_CMD_STOP_TRANSACTION , 0x00 );
    return 0;
}
