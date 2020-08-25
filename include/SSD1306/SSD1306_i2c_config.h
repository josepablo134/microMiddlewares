#ifndef _SSD1306_I2C_CONFIG_H_
#define _SSD1306_I2C_CONFIG_H_

#ifdef __cplusplus
    extern "C"
    {
#endif

    #include    <I2C/DriverI2C.h>
    #include    <I2C/DriverI2C_config.h>

    #define     SSD1306_I2C             EK_TM4C1294XL_I2C8
    #define     SSD1306_I2C_SPEED       DriverI2C_IOCTL_CMD_SETSPEED_100K
    #define     SSD1306_I2C_SLAVE_ADDR  0x3C
    #define     SSD1306_WIDTH           (128)
    #define     SSD1306_HEIGTH          (64)
    #define     SSD1306_BUFFER_SIZE     (SSD1306_WIDTH * SSD1306_HEIGTH)

#ifdef __cplusplus
    }
#endif


#endif
