cmake_minimum_required(VERSION 3.16)


project(ldog)


add_executable(
    ${PROJECT_NAME}
    src/FileSinkImpl.cc
    src/LogManager.cc
    src/LogMessage.cc
    src/ConsoleSinkImpl.cc
    app/main.cc
)

target_include_directories(${PROJECT_NAME} PRIVATE inc/)



