cmake_minimum_required(VERSION 3.16)


project(ldog)


add_executable(
    ${PROJECT_NAME}
    src/sinks/FileSinkImpl.cc
    src/sinks/ConsoleSinkImpl.cc
    src/protection/SafeFile.cc
    src/protection/SafeSocket.cc
    src/tel_src/FileTelemetrySourceImpl.cc
    src/tel_src/SocketTelemetrySourceImpl.cc
    src/LogManager.cc
    src/LogMessage.cc
    app/main.cc
)

target_include_directories(${PROJECT_NAME} PRIVATE inc/)



