cmake_minimum_required(VERSION 3.5.0)
project(piController VERSION 0.1.0 LANGUAGES C CXX)

# Find libgpiod
find_library(GPIOD_LIBRARY gpiodcxx)
if(NOT GPIOD_LIBRARY)
    message(FATAL_ERROR "libgpiod not found")
endif()

# Link libraries

# Example using add_library for an imported library
add_library(gpiodcxx SHARED IMPORTED )
set_target_properties(gpiodcxx PROPERTIES IMPORTED_LOCATION /usr/local/lib/libgpiodcxx.so) # or .a, .lib
# target_link_libraries(PRIVATE MyPrebuiltLib)


include(CTest)
enable_testing()

add_executable(piController main.cpp
    fdComm.cpp
    GPIOaccess.cpp
    Display.cpp
    colorCron.cpp
    RGB.cpp
    stopWatch.cpp
    Digit.cpp
    I2Cbus.cpp
    Socket.cpp
    ServerSocket.cpp
    ClientSocket.cpp
    MessageBuffer.cpp
    )

target_link_libraries(piController gpiodcxx)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
add_compile_definitions(TESTING=1)
#set_target_properties()