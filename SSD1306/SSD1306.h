#ifndef SSD1306_H_
#define SSD1306_H_

#ifdef __cplusplus
    extern "C"
    {
#endif
    #include <stdint.h>
    /// This as a return should represent a failure
    #define SSD1306_ERROR                           (-1)

    /// This are not a IOCTL commands
    #define SSD1306_IOCTL_CMD_MASK                  ~(0x00FF)
    #define SSD1306_IOCTL_CMD_DATA                  0x00FF

    /// Power status OFF
    #define SSD1306_IOCTL_POWER_SET_OFF             0x0100

    /// Power status ON
    #define SSD1306_IOCTL_POWER_SET_ON              0x0101

    /// Set the default screen configuration
    #define SSD1306_IOCTL_DEFAULT_CONFG             0x0200

    /// Send a single command byte
    #define SSD1306_IOCTL_SENDCMD_BYTE              0x0300

    /// Send a command byte array (requires a bit OR)
    //  Example
    //  @code{.c}
    //      uint8_t buff[3] = { CMD1 , CMD2 , CMD3 };
    //      SSD1306_ioctl( SSD1306_IOCTL_SENDCMD_ARRY | 0x03 , buff )
    //  @endcode
    #define SSD1306_IOCTL_SENDCMD_ARRY              0x0400

    /// Clear screen
    #define SSD1306_IOCTL_CLC                       0x0500
    /// Default configuration
    #define SSD1306_IOCTL_DEF_CONF                  0x0600

    extern void SSD1306_init();
    extern int SSD1306_open();
    extern int SSD1306_write( const void *buffer , uint32_t size );
    extern int SSD1306_ioctl( uint32_t config , void* buffer);

#ifdef __cplusplus
    }
#endif

#endif
