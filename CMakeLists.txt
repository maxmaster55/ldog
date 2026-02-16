cmake_minimum_required(VERSION 3.16)

project(ldog)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CMAKE_C_COMPILER aarch64-rpi3-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-rpi3-linux-gnu-g++)

set(CMAKE_SYSROOT /home/maxmaster/x-tools/aarch64-rpi3-linux-gnu/aarch64-rpi3-linux-gnu/sysroot)

set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)



# CommonAPI
find_package(CommonAPI REQUIRED)
find_package(CommonAPI-SomeIP REQUIRED)
find_package(vsomeip3 REQUIRED)


set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_library(ldog_lib STATIC 
    src/sinks/FileSinkImpl.cc
    src/sinks/ConsoleSinkImpl.cc
    src/protection/SafeFile.cc
    src/protection/SafeSocket.cc
    src/tel_src/FileTelemetrySourceImpl.cc
    src/tel_src/SocketTelemetrySourceImpl.cc
    src/tel_src/SomeIPTelemetrySourceImpl.cc
    src/LogManager.cc
    src/LogMessage.cc
    src/LogDog.cc
)

target_link_libraries(ldog_lib
    PUBLIC
    CommonAPI
    CommonAPI-SomeIP
    vsomeip3
)


target_include_directories(ldog_lib PUBLIC 
    ${CMAKE_SOURCE_DIR}/inc/
    ${CMAKE_SOURCE_DIR}/Gen/src-gen
)

set(GENERATED_SOMEIP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Gen/src-gen/v1/ldog/gpu/GpuUsageDataSomeIPDeployment.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Gen/src-gen/v1/ldog/gpu/GpuUsageDataSomeIPProxy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Gen/src-gen/v1/ldog/gpu/GpuUsageDataSomeIPStubAdapter.cpp
)

add_executable(
    ${PROJECT_NAME}
    app/main.cc
    ${GENERATED_SOMEIP_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    ldog_lib
)



add_executable(
    ${PROJECT_NAME}_service
    app/service_test.cc
    ${GENERATED_SOMEIP_SOURCES}
)

target_link_libraries(${PROJECT_NAME}_service PRIVATE 
    ldog_lib
)


# enable_testing()

