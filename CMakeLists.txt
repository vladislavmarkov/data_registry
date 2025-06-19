cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 14)

project(
    data_registry
    VERSION 0.0.1
    LANGUAGES CXX)

option(BUILD_EXAMPLES "build example programs" OFF)

add_subdirectory(3rdparty)
add_subdirectory(tests)

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()

add_library(data_registry INTERFACE)
add_library(data::registry ALIAS data_registry)

target_include_directories(data_registry INTERFACE include)
