cmake_minimum_required(VERSION 3.16)


project(ldog)


add_executable(
    ${PROJECT_NAME}
    src/main.cc
)

target_include_directories(${PROJECT_NAME} PRIVATE inc/)



