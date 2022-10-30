cmake_minimum_required(VERSION 3.20)

project(stb_image)

add_library(stb_image STATIC
    "stb_image.c")

set_target_properties(stb_image PROPERTIES LINKER_LANGUAGE C)
target_include_directories(stb_image
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)