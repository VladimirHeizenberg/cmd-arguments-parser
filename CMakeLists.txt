cmake_minimum_required(VERSION 3.20)

project(
    cmd-arg-parser
    VERSION 1.0
    DESCRIPTION "CMD arguments parsing library"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)


add_subdirectory(lib)
add_subdirectory(bin)


enable_testing()
add_subdirectory(test)