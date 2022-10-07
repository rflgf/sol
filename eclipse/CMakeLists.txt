# eclipse #
project(eclipse CXX)

set(eclipse_src
	main.cpp
)

list(TRANSFORM eclipse_src PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/src/")

add_executable(eclipse ${eclipse_src})

target_link_libraries(eclipse sol)

target_include_directories (eclipse PRIVATE "${CMAKE_SOURCE_DIR}/sol" "${CMAKE_CURRENT_SOURCE_DIR}/src")
# /eclipse #
