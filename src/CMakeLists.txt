file(GLOB_RECURSE HOST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

add_library(pkr SHARED ${HOST_SOURCES})

target_include_directories(pkr PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)