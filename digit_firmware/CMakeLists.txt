# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Set the board type
# this file is in ...pico-sdk\src\boards\include\boards
set(PICO_BOARD_HEADER_DIRS ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/../lib)
#set(PICO_BOARD_HEADER_DIRS "C:\\projects\\Big-Digit-Display\\VSARM\\lib")
set(PICO_BOARD "seeed_xiao_rp2040")
#set(PICO_BOARD "pico")
# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

#Set name of project (as PROJECT_NAME) and C/C++ standards
project(DIGITRP2040 C CXX ASM )
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()



#include_directories(/home/bobostrom/projects/Big-Digit-Display/VSARM/lib/)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../lib/)
# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME}
    main.cpp
    Pixels.cpp
    Digit.cpp
    Core1.cpp
    ../lib/RGB.cpp
    
    Register.cpp
    Registers.cpp
    Flash.cpp
    functions.cpp
    ../lib/I2Cbus.cpp
    ../lib/Serial.cpp
)


# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include($ENV{PICO_SDK_PATH}/tools/CMakeLists.txt)





###########################

#Create C header file with the name <pio program>.pio.h
pico_generate_pio_header(${PROJECT_NAME}  
        ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio
)


##########################


# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})
# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    pico_stdlib
    pico_unique_id
    pico_binary_info
    pico_multicore
    pico_flash
    pico_bootrom
    hardware_i2c
    hardware_pio
    hardware_flash
    hardware_sync
    hardware_watchdog
    hardware_exception
)


pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)





# doesn't work...git command adds a new line
# add_custom_command(
#     COMMENT "CREATING FILE..."
#     TARGET ${PROJECT_NAME} PRE_BUILD
#     COMMAND ${CMAKE_COMMAND} -E echo_append $<1:"const char str1[] =  \\\""> > ${CMAKE_SOURCE_DIR}/versions.hpp
#     COMMAND git describe --abbrev=4 --dirty --always --tags >> ${CMAKE_SOURCE_DIR}/versions.hpp
#     # COMMAND ${CMAKE_COMMAND} -E echo_append $<1:" Wor\\\"ld"> >> ${CMAKE_SOURCE_DIR}/versions.hpp
#     COMMAND ${CMAKE_COMMAND} -E echo $<1:"World"> >> ${CMAKE_SOURCE_DIR}/versions.hpp
# )

# message("CMAKE_SYSTEM: ${CMAKE_HOST_SYSTEM_NAME}")
# message("CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM}")
#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_PROJECT_NAME}.uf2 d:)
message((STATUS) "CMAKE_HOST_SYSTEM_NAME: ${CMAKE_HOST_SYSTEM_NAME}")

if(${CMAKE_HOST_LINUX})
    add_custom_command(COMMENT "COPYING FILE..." TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_PROJECT_NAME}.uf2 /media/bobostrom/RPI-RP2)
else()
    add_custom_command(COMMENT "COPYING FILE..." TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_PROJECT_NAME}.uf2 d:)
endif()