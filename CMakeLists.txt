cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME KBSESA1)
project(${PROJECT_NAME})

# Compile and link all project files
file(GLOB ${CMAKE_PROJECT_NAME}_SRCS
        "*.cpp"
        "libs/*.cpp"
        )

file(GLOB ${CMAKE_PROJECT_NAME}_HDRS
        "*.h"
        "libs/*.h"
        )

# Configure upload target
set(${CMAKE_PROJECT_NAME}_BOARD uno)
set(${CMAKE_PROJECT_NAME}_PORT COM3 ) # Edit this

# Magic happens here!
generate_arduino_firmware(${CMAKE_PROJECT_NAME})
