################################################################################
# @file CMakeLists.txt
# @author Josepablo C (josepablo134@gmail.com)
################################################################################
set(COMPONENT_NAME TivaWare)

add_library(${COMPONENT_NAME} STATIC
		${CMAKE_CURRENT_LIST_DIR}/driverlib/adc.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/aes.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/can.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/comp.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/cpu.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/crc.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/des.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/eeprom.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/emac.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/epi.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/flash.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/fpu.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/gpio.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/hibernate.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/i2c.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/interrupt.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/lcd.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/mpu.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/onewire.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/pwm.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/qei.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/shamd5.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/ssi.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/sw_crc.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/sysctl.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/sysexc.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/systick.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/timer.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/uart.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/udma.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/usb.c
		${CMAKE_CURRENT_LIST_DIR}/driverlib/watchdog.c
)

target_include_directories(${COMPONENT_NAME} PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/
		${CMAKE_CURRENT_LIST_DIR}/driverlib
		${CMAKE_CURRENT_LIST_DIR}/driverlib/inc
)

