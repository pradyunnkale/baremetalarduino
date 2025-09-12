cmake_minimum_required(VERSION 3.10)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)
set(CMAKE_C_COMPILER avr-gcc)

project(arduino_baremetal C)

set(MCU atmega328p)
set(F_CPU 16000000UL)

set(CMAKE_C_FLAGS "-mmcu=${MCU} -DF_CPU=${F_CPU} -Os -Wall -Wextra")

add_executable(main main.c)

add_custom_command(TARGET main POST_BUILD
    COMMAND avr-objcopy -O ihex main main.hex
    COMMENT "Creating hex file")

add_custom_target(upload
    COMMAND avrdude -c arduino -p $}{MCU} -P /dev/ttyACM0 -b 115200 -U flash:w:main.hex
    DEPENDS main
    COMMENT "Uploading to Arduino")

find_path(AVR_INCLUDE_DIR avr/io.h
    PATHS 
    /usr/lib/avr/include
    /usr/avr/include
    /opt/local/avr/include
    /usr/local/avr/include
)

if(AVR_INCLUDE_DIR)
    include_directories(${AVR_INCLUDE_DIR})
else()
    message(FATAL_ERROR "Could not find AVR include directory")
endif()
