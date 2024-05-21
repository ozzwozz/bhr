# == NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
   set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(PICO_SDK_PATH ${USERHOME}/.pico-sdk/sdk/1.5.1)
set(PICO_TOOLCHAIN_PATH ${USERHOME}/.pico-sdk/toolchain/13_2_Rel1)
if(WIN32)
    set(pico-sdk-tools_DIR ${USERHOME}/.pico-sdk/tools/1.5.1)
    include(${pico-sdk-tools_DIR}/pico-sdk-tools-config.cmake)
    include(${pico-sdk-tools_DIR}/pico-sdk-tools-config-version.cmake)
endif()
# ====================================================================================
# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
   set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(PICO_SDK_PATH ${USERHOME}/.pico-sdk/sdk/1.5.1)
set(PICO_TOOLCHAIN_PATH ${USERHOME}/.pico-sdk/toolchain/13_2_Rel1)
if(WIN32)
    set(pico-sdk-tools_DIR ${USERHOME}/.pico-sdk/tools/1.5.1)
    include(${pico-sdk-tools_DIR}/pico-sdk-tools-config.cmake)
    include(${pico-sdk-tools_DIR}/pico-sdk-tools-config-version.cmake)
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(attenuator_controller C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add all your header files here
set(HEADER_FILES
    include/adc.h
    include/DS1682.h
    include/I2CDevice.h
    include/M24M02.h
    include/MAX31725.h
    include/PCA9554.h
    include/SI53361.h
    include/SPIDevice.h
    include/uart.h
)

# Add all your source files here
set(SOURCE_FILES
    src/adc.cpp
    src/attenuator_controller.cpp
    src/DS1682.cpp
    src/I2CDevice.cpp
    src/M24M02.cpp
    src/MAX31725.cpp
    src/PCA9554.cpp
    src/SPIDevice.cpp
    src/SI53361.cpp
    src/uart.cpp
)

# Include the directories containing the header files
include_directories(include)

# Add executable. Default name is the project name, version 0.1

# add_executable(attenuator_controller attenuator_controller.cpp )
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

pico_set_program_name(attenuator_controller "attenuator_controller")
pico_set_program_version(attenuator_controller "0.1")

# Generate PIO header
pico_generate_pio_header(attenuator_controller ${CMAKE_CURRENT_LIST_DIR}/blink.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(attenuator_controller 1)
pico_enable_stdio_usb(attenuator_controller 0)

# Add the standard library to the build
target_link_libraries(attenuator_controller
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(attenuator_controller PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries(attenuator_controller 
        hardware_spi
        hardware_i2c
        hardware_pio
        hardware_timer
        hardware_watchdog
        hardware_clocks
        hardware_adc
        )

pico_add_extra_outputs(attenuator_controller)

