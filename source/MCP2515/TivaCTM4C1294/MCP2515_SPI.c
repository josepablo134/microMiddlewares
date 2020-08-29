/// Include Middleware description
//  requires a compilation environment that includes
//  the directory "include/"
#include <MCP2515/MCP2515.h>
#include <MCP2515/MCP2515_regmap.h>
#include <MCP2515/TivaCTM4C1294/MCP2515_config.h>

#include <SPI/DriverSPI.h>
#include <SPI/TivaCTM4C1294/DriverSPI_config.h>

/// Dependencias locales
#include <stdbool.h>
#include <stdint.h>

void MCP2515_init(void){
	DriverSPI_init();
}
///	Prepare controller to any transaction
int MCP2515_open(unsigned int device ){
	return DriverSPI_open( device );
}
///	Send a IOCTL order
int MCP2515_ioctl( int fd , unsigned int config , void* buffer ){
    return -1;
}
///	Send a new package
int MCP2515_write( int fd , const void* buffer, unsigned int size ){
    return -1;
}
///	Read any available package
int MCP2515_read( int fd , void* buffer, unsigned int size ){
    return -1;
}
///	Release controller
void MCP2515_close( int fd ){
}
