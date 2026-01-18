cmake_minimum_required(VERSION 3.16)

project(ldog)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GoogleTest setup
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(ldog_lib STATIC 
    src/sinks/FileSinkImpl.cc
    src/sinks/ConsoleSinkImpl.cc
    src/protection/SafeFile.cc
    src/protection/SafeSocket.cc
    src/tel_src/FileTelemetrySourceImpl.cc
    src/tel_src/SocketTelemetrySourceImpl.cc
    src/LogManager.cc
    src/LogMessage.cc)
target_include_directories(ldog_lib PUBLIC inc/)

add_executable(
    ${PROJECT_NAME}
    app/main.cc
)

target_link_libraries(${PROJECT_NAME} PRIVATE ldog_lib)

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

