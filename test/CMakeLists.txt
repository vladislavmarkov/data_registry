include(CTest)
include(${DOCTEST_CMAKE_HELPER})
enable_testing()

add_executable(test_demo src/test_demo.cpp src/stored_demo.cpp)
target_link_libraries(test_demo PRIVATE doctest::doctest fmt::fmt data::registry)
target_compile_options(test_demo PRIVATE -Wall -Wextra -Werror -Wpedantic)

doctest_discover_tests(test_demo)

add_executable(crvalue_accessor src/crvalue_accessor.cpp
                                src/stored_crvalue_accessor.cpp)

target_link_libraries(crvalue_accessor PRIVATE doctest::doctest fmt::fmt
                                               data::registry)

target_compile_options(crvalue_accessor PRIVATE -Wall -Wextra -Werror
                                                -Wpedantic)

doctest_discover_tests(crvalue_accessor)
