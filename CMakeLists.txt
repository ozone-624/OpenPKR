cmake_minimum_required(VERSION 3.15)
project(hello-pkr C CXX)

set(CMAKE_C_STANDARD 99)

add_subdirectory(src)

add_executable(hello-pkr test/test.c)
target_include_directories(hello-pkr PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(hello-pkr PRIVATE pkr)

set_target_properties(pkr PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set_target_properties(hello-pkr PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)