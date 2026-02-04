cmake_minimum_required(VERSION 3.16)

project(ldog)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# CommonAPI
find_package(CommonAPI REQUIRED)
find_package(CommonAPI-SomeIP REQUIRED)
find_package(vsomeip3 REQUIRED)
find_package(GTest REQUIRED)


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
    src/LogMessage.cc)

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

enable_testing()

add_executable(
    tests
    tests/msg_tests.cc
    tests/log_manager_tests.cc
    tests/message_extended_tests.cc
    tests/sink_tests.cc
)

target_include_directories(tests PRIVATE inc/)

target_link_libraries(
    tests
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
    ldog_lib
)

