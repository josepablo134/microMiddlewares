#ifndef DRIVERS_SSD1306_I2C_H_
#define DRIVERS_SSD1306_I2C_H_

#ifdef __cplusplus
    extern "C"
    {
#endif

    /// Public prototypes definitions
    #include <SSD1306/SSD1306.h>
    #include <stdint.h>

    //////////////////////////////////
    // Private prototypes definitions
    //////////////////////////////////

    extern int SSD1306_defaultConfig(void);
	extern int SSD1306_clc(void);
	extern int SSD1306_setPage( uint8_t pageInit , uint8_t pageEnd );
	extern int SSD1306_setColumn( uint8_t columnInit , uint8_t columnEnd);
    extern int SSD1306_writeCommand( uint8_t command );
    extern int SSD1306_writeCommandList( void* buff , uint8_t size );
    extern int SSD1306_powerOn(void);
    extern int SSD1306_powerOff(void);

#ifdef __cplusplus
    }
#endif

#endif
