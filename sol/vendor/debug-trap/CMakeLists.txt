cmake_minimum_required(VERSION 3.20)

project(debug-trap)

add_library(debug-trap STATIC
    "debug-trap.c")

set_target_properties(debug-trap PROPERTIES LINKER_LANGUAGE C)
target_include_directories(debug-trap
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)