# example app #
project(example_app CXX)

set(example_app_src
	main.cpp
)

list(TRANSFORM example_app_src PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/src/")

add_executable(example_app ${example_app_src})

target_link_libraries(example_app sol)

target_include_directories(example_app PRIVATE "${CMAKE_SOURCE_DIR}/sol" "${CMAKE_CURRENT_SOURCE_DIR}/src")
# /example app #