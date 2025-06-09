add_executable(tests src/tests.cpp src/stored_data.cpp)

target_link_libraries(tests PRIVATE doctest::doctest fmt::fmt data::registry)

target_compile_options(tests PRIVATE -Wall -Wextra -Werror -Wpedantic
                                     -save-temps)
