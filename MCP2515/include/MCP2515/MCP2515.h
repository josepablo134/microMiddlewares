#ifndef MCP2515_H_
#define MCP2515_H_

#ifdef __cplusplus
    extern "C"
    {
#endif

	/// Dependencias locales
	#include <stdbool.h>
	#include <stdint.h>

    /// This as a return should represent a failure
	#define MCP2515_ERROR                           (-1)

    /// This is not a IOCTL command
    #define MCP2515_IOCTL_CMD_MASK            	    0xFFFFFF00

	///	This is not a IOCTL command
    #define MCP2515_IOCTL_DATA_MASK            	    0x000000FF

    /// Reset device
    #define MCP2515_IOCTL_RESET                     0x00000100

    /// Sending command and data
    #define MCP2515_IOCTL_WRITE_CMD                 0x00000200

    /// Sending command and reading data
    #define MCP2515_IOCTL_READ_CMD                  0x00000300

    /// Request to send frame buffer
    #define MCP2515_IOCTL_RTS_BUFFER                0x00000400
    #define MCP2515_IOCTL_RTS_ALL                   0x00000004

    /// Request one shot send
    #define MCP2515_IOCTL_ONE_SHOT_MODE             0x00000500
    #define MCP2515_IOCTL_ONE_SHOT_MODE_DISABLED    0x00000500
    #define MCP2515_IOCTL_ONE_SHOT_MODE_ENABLED     0x00000501

    struct MCP2515_command{
        uint8_t cmd[2];
        uint8_t cmdLen;
        uint8_t *params;
        uint8_t paramsLen;
    };

    /// Almost the same as SocketCAN
    /// https://www.kernel.org/doc/html/latest/networking/can.html
    typedef union MCP2515_can_id_t{
        struct {
            unsigned    CAN_ID:29;/// ID code, bits (0 .. 28)
            unsigned    CAN_ID_ERR:1;/// Error Frame, bits (29)
            unsigned    CAN_ID_RTR:1;/// Request to send, bits (30)
            unsigned    CAN_ID_EFF:1;/// Extended Frame Format, bits (31)
        };
        struct {
            unsigned    CAN_ID_EXT:18;///bits (10 .. 17)
            unsigned    CAN_ID_STD:11;///bits (0 .. 10)
        };
        uint32_t    ID;/// Raw CAN ID code
    }MCP2515_can_id_t;

    typedef enum MCP2515_tx_buffer_id{
        MCP2515_txbuff0=0,
        MCP2515_txbuff1,
        MCP2515_txbuff2,
    }MCP2515_tx_buffer_id;

    typedef enum MCP2515_rx_buffer_id{
        MCP2515_rxbuff0=0,
        MCP2515_rxbuff1,
    }MCP2515_rx_buffer_id;

    typedef union MCP2515_buffer_id_t{
        MCP2515_tx_buffer_id    txBufferId;
        MCP2515_rx_buffer_id    rxBufferId;
    }MCP2515_buffer_id_t;

    struct MCP2515_can_frame{
        MCP2515_can_id_t     can_id;//< 32 bit CAN_ID + EFF/RTR/ERR flags
        uint8_t     can_dlc;//< frame payload length in byte (0 .. 8)
        uint8_t     data[8] __attribute__((aligned(8)));
        MCP2515_buffer_id_t    buffer_id;
    };

	///	Initialize controller registers
	extern void MCP2515_init(void);
	///	Prepare controller to any transaction
	extern int MCP2515_open(unsigned int device );
	///	Send a IOCTL order
	extern int MCP2515_ioctl( int fd , unsigned int config , void* buffer );
	///	Send a new package
	extern int MCP2515_write( int fd , const void* buffer, unsigned int size );
	///	Read any available package
	extern int MCP2515_read( int fd , void* buffer, unsigned int size );
	///	Release controller
	extern void MCP2515_close( int fd );

#ifdef __cplusplus
    }
#endif

#endif
