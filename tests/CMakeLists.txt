include(CTest)
enable_testing()

add_executable(demo src/demo.cpp src/stored_demo.cpp)
target_link_libraries(demo PRIVATE doctest::doctest fmt::fmt data::registry)
target_compile_options(demo PRIVATE -Wall -Wextra -Werror -Wpedantic)

add_test(NAME demo COMMAND demo)

add_executable(crvalue_accessor src/crvalue_accessor.cpp
                                src/stored_crvalue_accessor.cpp)

target_link_libraries(crvalue_accessor PRIVATE doctest::doctest fmt::fmt
                                               data::registry)

target_compile_options(crvalue_accessor PRIVATE -Wall -Wextra -Werror
                                                -Wpedantic)

add_test(NAME crvalue_accessor COMMAND crvalue_accessor)
