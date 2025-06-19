add_executable(statics src/main.cpp)

target_link_libraries(statics PRIVATE data::registry fmt::fmt)
