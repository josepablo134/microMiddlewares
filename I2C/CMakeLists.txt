################################################################################
# @file CMakeLists.txt
# @author Josepablo C (josepablo134@gmail.com)
################################################################################
set(COMPONENT_NAME I2C)
set(TIVAC_MODEL TivaCTM4C1294)

add_library(${COMPONENT_NAME}_${TIVAC_MODEL} STATIC
		${CMAKE_CURRENT_LIST_DIR}/src/${TIVAC_MODEL}/Driver${COMPONENT_NAME}.c
		${CMAKE_CURRENT_LIST_DIR}/src/${TIVAC_MODEL}/Driver${COMPONENT_NAME}_config.c
)

target_include_directories(${COMPONENT_NAME}_${TIVAC_MODEL} PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/include
)

add_library(${COMPONENT_NAME}::${TIVAC_MODEL} ALIAS ${COMPONENT_NAME}_${TIVAC_MODEL})

target_link_libraries( ${COMPONENT_NAME}_${TIVAC_MODEL} TivaWare )

