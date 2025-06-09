cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 14)

project(
    data_registry
    VERSION 0.0.1
    LANGUAGES CXX)

add_subdirectory(3rdparty)
add_subdirectory(tests)

add_library(data_registry INTERFACE)
add_library(data::registry ALIAS data_registry)

target_include_directories(data_registry INTERFACE include)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

# set(GENERATED_DATA ${CMAKE_CURRENT_SOURCE_DIR}/include/data/detail/for_each.inl)
#
# add_custom_command(
#     OUTPUT ${GENERATED_DATA}
#     COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/make_for_each_N.py 1000 ${GENERATED_DATA}
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/make_for_each_N.py
#     COMMENT "Running data generator"
#     VERBATIM)

target_include_directories(data_registry INTERFACE ${CMAKE_CURRENT_BINARY_DIR})

target_sources(data_registry PRIVATE ${GENERATED_DATA})
