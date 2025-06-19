include(CTest)
include(${DOCTEST_CMAKE_HELPER})
enable_testing()

add_executable(tests src/tests.cpp)

target_link_libraries(tests PRIVATE doctest::doctest data::registry)
target_compile_options(tests PRIVATE -Wall -Wextra -Werror -Wpedantic)

doctest_discover_tests(tests)
