include(CTest)
include(${DOCTEST_CMAKE_HELPER})
enable_testing()

set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

add_executable(tests src/tests.cpp)

target_link_libraries(tests PRIVATE doctest::doctest data::registry)
target_compile_options(tests PRIVATE -Wall -Wextra -Werror -Wpedantic)

doctest_discover_tests(tests)

set(GEN_COUNT 2000)
set(GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GEN_DIR})

# data::registry entries generator files
set(GENERATED_ENTRY_FILES
    ${GEN_DIR}/generated_${GEN_COUNT}_entries.inl
    ${GEN_DIR}/generated_${GEN_COUNT}_store.inl
    ${GEN_DIR}/generated_${GEN_COUNT}_check.inl)

# raw static variable files
set(GENERATED_RAW_FILES ${GEN_DIR}/generated_${GEN_COUNT}_raw_static_entries.inl
                        ${GEN_DIR}/generated_${GEN_COUNT}_raw_static_check.inl)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

add_custom_command(
    OUTPUT ${GENERATED_ENTRY_FILES}
    COMMAND
        ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/entries_generator.py
        ${GEN_COUNT} -o ${GEN_DIR}
    BYPRODUCTS ${GENERATED_ENTRY_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating ${GEN_COUNT} registration snippets"
    VERBATIM)

add_custom_target(generate_entries ALL DEPENDS ${GENERATED_ENTRY_FILES})

add_executable(data-registry src/size_registry.cpp)

target_sources(data-registry PRIVATE FILE_SET HEADERS BASE_DIRS ${GEN_DIR}
                                     FILES ${GENERATED_ENTRY_FILES})

add_dependencies(data-registry generate_entries)

target_compile_definitions(
    data-registry
    PRIVATE -DGENERATED_ENTRIES="generated_${GEN_COUNT}_entries.inl"
            -DGENERATED_STORES="generated_${GEN_COUNT}_store.inl"
            -DGENERATED_CHECKS="generated_${GEN_COUNT}_check.inl")

target_link_libraries(data-registry PRIVATE data::registry)
target_compile_options(data-registry PRIVATE -Wall -Wextra -Werror -Wpedantic)

add_custom_command(
    OUTPUT ${GENERATED_RAW_FILES}
    COMMAND
        ${Python3_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/raw_statics_generator.py ${GEN_COUNT} -o
        ${GEN_DIR}
    BYPRODUCTS ${GENERATED_RAW_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating ${GEN_COUNT} registration snippets"
    VERBATIM)

add_custom_target(generate_raw_statics ALL DEPENDS ${GENERATED_RAW_FILES})

add_executable(pure-statics src/size_statics.cpp)

target_sources(pure-statics PRIVATE FILE_SET HEADERS BASE_DIRS ${GEN_DIR} FILES
                                    ${GENERATED_RAW_FILES})

add_dependencies(pure-statics generate_raw_statics)

target_compile_definitions(
    pure-statics
    PRIVATE -DGENERATED_STATICS="generated_${GEN_COUNT}_raw_static_entries.inl"
            -DGENERATED_CHECKS="generated_${GEN_COUNT}_raw_static_check.inl")

target_link_libraries(pure-statics PRIVATE data::registry)
target_compile_options(pure-statics PRIVATE -Wall -Wextra -Werror -Wpedantic)
