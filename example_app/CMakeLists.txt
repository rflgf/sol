# example app #
project(example_app CXX)

set(example_app_src
	main.cpp
)

list(TRANSFORM example_app_src PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/src/")

add_executable(example_app ${example_app_src})

target_link_libraries(example_app sol)

target_include_directories(example_app PRIVATE "${CMAKE_SOURCE_DIR}/sol" "${CMAKE_CURRENT_SOURCE_DIR}/src")

set_target_properties(example_app PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_SOURCE_DIR}/bin/Debug/example_app
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release/example_app
        ARCHIVE_OUTPUT_DIRECTORY_DIST    ${CMAKE_SOURCE_DIR}/bin/Dist/example_app
        LIBRARY_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_SOURCE_DIR}/bin/Debug/example_app
        LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release/example_app
        LIBRARY_OUTPUT_DIRECTORY_DIST    ${CMAKE_SOURCE_DIR}/bin/Dist/example_app
        RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_SOURCE_DIR}/bin/Debug/example_app
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release/example_app
        RUNTIME_OUTPUT_DIRECTORY_DIST    ${CMAKE_SOURCE_DIR}/bin/Dist/example_app
)
# /example app #