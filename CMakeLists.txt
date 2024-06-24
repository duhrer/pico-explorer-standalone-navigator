cmake_minimum_required(VERSION 3.12)

set(NAME pico-explorer-standalone-navigator)

include(pico_sdk_import.cmake)
include(pimoroni_pico_import.cmake)

project(${NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

include(common/pimoroni_i2c)
include(common/pimoroni_bus)
include(libraries/pico_explorer/pico_explorer)
include(drivers/button/button)

# Add your source files
add_executable(${NAME})

target_sources(${NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/usb_descriptors.c
)

# Make sure TinyUSB can find tusb_config.h
target_include_directories(${NAME} PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/src
)

# Link required libraries
target_link_libraries(
    ${NAME}
    PUBLIC
    pico_stdlib
    pico_explorer
    pico_graphics
    st7789
    button
    pico_unique_id
    tinyusb_device
    tinyusb_board
)

# Uncomment this line to enable fix for Errata RP2040-E5 (the fix requires use of GPIO 15)
# target_compile_definitions(${NAME} PUBLIC PICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1)

# create map/bin/hex file etc.
pico_add_extra_outputs(${NAME})

# Set up files for the release packages
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.uf2
    ${CMAKE_CURRENT_LIST_DIR}/README.md
    DESTINATION .
)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "ZIP" "TGZ")
include(CPack)

# Disable USB serial and enable UART serial
# pico_enable_stdio_usb(${PROJECT_NAME} 0)
# pico_enable_stdio_uart(${PROJECT_NAME} 1)
