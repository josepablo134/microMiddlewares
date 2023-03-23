################################################################################
# @file CMakeLists.txt
# @author Josepablo C (josepablo134@gmail.com)
################################################################################
cmake_minimum_required( VERSION 3.18.4 )

project( microMiddlewares
		DESCRIPTION "Project template for TivaC microcontroller"
		VERSION 1.0.0
		LANGUAGES C ASM
)

set(COMPONENT_NAME MicroMiddlewares)
set(TIVAC_MODEL TivaCTM4C1294)

add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/TivaWare )
add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/I2C )
add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/SPI )

add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/BUS8080 )

add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/ILI9341 )
add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/SSD1306 )
add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/MCP2515 )

add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/Misc )
add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/simpleGFX )
add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/System )

