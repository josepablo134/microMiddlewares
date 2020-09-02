/// Include Middleware description
//  requires a compilation environment that includes
//  the directory "include/"
#include <MCP2515/MCP2515.h>
#include <MCP2515/MCP2515_regmap.h>
#include <MCP2515/TivaCTM4C1294/MCP2515_config.h>

#include <SPI/DriverSPI.h>
#include <SPI/TivaCTM4C1294/DriverSPI_config.h>

/// TivaWare DriverLib
#include <inc/hw_memmap.h>
#include <driverlib/gpio.h>
#include <driverlib/rom_map.h>

/// Dependencias locales
#include <stdbool.h>
#include <stdint.h>

#define SPI_ENABLE      GPIOPinWrite( GPIO_PORTD_BASE , GPIO_PIN_2 , 0 )
#define SPI_DISABLE     GPIOPinWrite( GPIO_PORTD_BASE , GPIO_PIN_2 , GPIO_PIN_2 )

void MCP2515_init(void){
	DriverSPI_init();
}
///	Prepare controller to any transaction
int MCP2515_open(unsigned int device ){
	if( device >= TM4C1294XL_MCP2515_COUNT ){
		return -1;
	}

	int fd = DriverSPI_open( MCP2515_devices[ device ] );
	if( fd < 0 ){ return -1; }

    struct DriverSPI_mode mode;
    mode.bitrate = 10000000;
    mode.polarity = DriverSPI_SPI_MODE_POL_0;
    mode.phase = DriverSPI_SPI_MODE_PHA_0;

    /// Setting SPI operation mode
    DriverSPI_ioctl( fd , DriverSPI_IOCTL_CONF_MODE , &mode );

    /// Overwritte SPI_SS for GPIO MODE
    /// Manually control the GPIO SS/CS PIN
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2);
    GPIOPadConfigSet( GPIO_PORTD_BASE , GPIO_PIN_2 , GPIO_STRENGTH_12MA , GPIO_PIN_TYPE_STD );
    GPIOPinWrite( GPIO_PORTD_BASE , GPIO_PIN_2 , GPIO_PIN_2 );

	return fd;
}
///	Send a IOCTL order
int MCP2515_ioctl( int fd , unsigned int config , void* buffer ){
    switch( config & MCP2515_IOCTL_CMD_MASK ){
        case MCP2515_IOCTL_RESET:{
            SPI_ENABLE;
                uint8_t rstCommand = MCP2515_CMD_RESET;
                DriverSPI_write( fd, &rstCommand, 1);
            SPI_DISABLE;
            return 0;
        }
        case MCP2515_IOCTL_WRITE_CMD:{
            if( !buffer ){ return -1; }
            struct MCP2515_command* cmd = (struct MCP2515_command*) buffer;

            /// CS Enable
            SPI_ENABLE;
                DriverSPI_write( fd, &(cmd->cmd), cmd->cmdLen );
                DriverSPI_write( fd, cmd->params, cmd->paramsLen);
            /// CS Disable
            SPI_DISABLE;

            return 0;
        }
        case MCP2515_IOCTL_READ_CMD:{
            if( !buffer ){ return -1; }
            struct MCP2515_command* cmd = (struct MCP2515_command*) buffer;

            /// CS Enable
            SPI_ENABLE;
                DriverSPI_write( fd, &(cmd->cmd), cmd->cmdLen );
                DriverSPI_read( fd, cmd->params, cmd->paramsLen);
            /// CS Disable
            SPI_DISABLE;

            return 0;
        }
        case MCP2515_IOCTL_RTS_BUFFER:{
            uint8_t command;
                switch( config & MCP2515_IOCTL_DATA_MASK ){
                    case MCP2515_txbuff0:
                        command = MCP2515_CMD_RTS_TX_0;
                        break;
                    case MCP2515_txbuff1:
                        command = MCP2515_CMD_RTS_TX_1;
                        break;
                    case MCP2515_txbuff2:
                        command = MCP2515_CMD_RTS_TX_2;
                        break;
                    case MCP2515_IOCTL_RTS_ALL:
                        command = MCP2515_CMD_RTS_TX_ALL;
                        break;
                    default:
                        return -1;
                }
            /// CS Enable
            SPI_ENABLE;
                DriverSPI_write( fd, &command, 1 );
            /// CS Disable
            SPI_DISABLE;
            return 0;
        }
        case MCP2515_IOCTL_ONE_SHOT_MODE:{
            uint8_t command[] = {MCP2515_CMD_BIT_MODIFY,
                                 MCP2515_CAN_CTRL_REG,
                                 MCP2515_OSM_MASK,
                                 MCP2515_OSM_ENABLED
            };
            /// Request to disable One-shot mode
            if( !(config & MCP2515_IOCTL_DATA_MASK) ){
                command[3] = MCP2515_OSM_DISABLED;
            }
            /// CS Enable
            SPI_ENABLE;
                DriverSPI_write( fd, command, sizeof(command) );
            /// CS Disable
            SPI_DISABLE;
            return 0;
        }
    }
    return -1;
}
///	Send a new package
int MCP2515_write( int fd , const void* buffer, unsigned int size ){
    if(! buffer ){return-1;}
    if( size < sizeof(struct MCP2515_can_frame) ){return -1;}
    struct MCP2515_can_frame *frm = (struct MCP2515_can_frame*) buffer;

    if(frm->can_dlc >= MCP2515_MAX_FRAME_DATA_LENGTH){ return -1; }

    int wrCounter;
    uint8_t mcp_load_instruction;
    switch( frm->buffer_id.txBufferId ){
        case MCP2515_txbuff0:
            mcp_load_instruction = MCP2515_CMD_LOAD_TX_BUFF_0;
            break;
        case MCP2515_txbuff1:
            mcp_load_instruction = MCP2515_CMD_LOAD_TX_BUFF_1;
            break;
        case MCP2515_txbuff2:
            mcp_load_instruction = MCP2515_CMD_LOAD_TX_BUFF_2;
            break;
        default:
            return -1;
    }

    /// CS Enable
    SPI_ENABLE;
        /// Load Frame to send
        wrCounter = DriverSPI_write( fd, &mcp_load_instruction, 1 );

        /// According with the datasheet, the load instruction
        //  allows writing TX buffer in the next order:
        //  IDH, IDL, ID8, ID0, DLC, D0..D7
        uint8_t canId[4];

            //  IDH -> Standard ID higher bits (10 .. 3) bit.
            canId[0] = (frm->can_id.CAN_ID_STD)>>3;

            //  IDL -> Standard ID lower bits (2 .. 0) bits
            canId[1] = (frm->can_id.CAN_ID_STD)<<5;
            /// Extended frame format
            if( frm->can_id.CAN_ID_EFF ){
                canId[1] |= MCP2515_TX_BUFF_ID_HIGH_EXIDE;///Set EXIDE Flag
            }
            ///Set extended ID bits (17 .. 16) bits
            canId[1] |= ( (frm->can_id.CAN_ID_EXT)>>16 ) & 0x3;

            /// Set the rest of the Extended ID
            canId[2] = (frm->can_id.CAN_ID_EXT>>8);//ID8
            canId[3] = (frm->can_id.CAN_ID_EXT);//ID0

        /// Send actual ID configuration
        wrCounter += DriverSPI_write( fd, canId , sizeof(canId) );

        /// Configure Data Length Code Register
        //  Set RTR bit if required
        canId[0] = frm->can_dlc | ( frm->can_id.CAN_ID_RTR << 6 );
        wrCounter += DriverSPI_write( fd, canId , 1 );

        /// Send data bytes
        wrCounter += DriverSPI_write( fd, frm->data , frm->can_dlc );
    /// CS Disable
    SPI_DISABLE;

    return wrCounter;
}
///	Read any available package
int MCP2515_read( int fd , void* buffer, unsigned int size ){
    if(! buffer ){return-1;}
    if( size < sizeof(struct MCP2515_can_frame) ){return -1;}
    struct MCP2515_can_frame *frm = (struct MCP2515_can_frame*) buffer;

    int rdCounter;
    uint8_t mcp_read_instruction;
    switch( frm->buffer_id.rxBufferId ){
        case MCP2515_rxbuff0:
            mcp_read_instruction = MCP2515_CMD_READ_RX_BUFF_0;
            break;
        case MCP2515_rxbuff1:
            mcp_read_instruction = MCP2515_CMD_READ_RX_BUFF_0;
            break;
        default:
            return -1;
    }

    /// CS Enable
    SPI_ENABLE;
        /// According with the datasheet, the read instruction
        //  allows reading RX buffer in the next order:
        //  IDH, IDL, ID8, ID0, DLC, D0..D7
        uint8_t canId[5];

        /// Read Frame command
        DriverSPI_write( fd, &mcp_read_instruction, 1 );

        /// Read IDH,IDL,ID8,ID0 and DLC
        rdCounter = DriverSPI_read( fd , canId , sizeof(canId) );

        /// CAN ID Needs reorder

        /// Setting Standard CAN ID
        frm->can_id.CAN_ID_STD = ((uint16_t)canId[0])<<3;
        frm->can_id.CAN_ID_STD |= canId[1]>>5;

        /// Extended Frame Format
        frm->can_id.CAN_ID_EFF = (canId[1] & MCP2515_RX_BUFF_ID_LOW_IDE)? 1: 0;

        /// Standard Frame Remote Transmit Request
        frm->can_id.CAN_ID_RTR = (canId[1] & MCP2515_RX_BUFF_ID_LOW_SRR)? 1: 0;

        if( frm->can_id.CAN_ID_EFF ){
            /// Extended Frame Remote Transmit Request
            frm->can_id.CAN_ID_RTR |= (canId[4] & MCP2515_RX_DLC_RTR)? 1: 0;

            /// Setting Extended CAN ID
            frm->can_id.CAN_ID_EXT = ((uint32_t)canId[1] & 0x03)<<16;
            frm->can_id.CAN_ID_EXT |= ((uint32_t)canId[2]) << 8;
            frm->can_id.CAN_ID_EXT |= canId[3];
        }else{
            frm->can_id.CAN_ID_EXT = 0;
        }

        /// Set Data Length Code
        frm->can_dlc = canId[4] & 0x0F;

        /// Validate Data Length Code
        if(frm->can_dlc >= MCP2515_MAX_FRAME_DATA_LENGTH){
            SPI_DISABLE;
            return -1;
        }

        /// Read Frame Bytes
        rdCounter += DriverSPI_read( fd , &frm->data , frm->can_dlc );
    /// CS Disable
    SPI_DISABLE;

    return rdCounter;
}
///	Release controller
void MCP2515_close( int fd ){
    DriverSPI_close(fd);
}
