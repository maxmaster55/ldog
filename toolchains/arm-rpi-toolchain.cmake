set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CMAKE_C_COMPILER aarch64-rpi3-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-rpi3-linux-gnu-g++)

# set(CMAKE_SYSROOT path/to/sysroot)

if(NOT DEFINED CMAKE_SYSROOT)
    message(NOTICE "use CMAKE_SYSROOT to specify the sysroot path in the cacheVariables section of the preset")
    message(FATAL_ERROR "SYSROOT variable not set")
else()
    message(NOTICE "Using sysroot: ${CMAKE_SYSROOT}")
endif()

set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

