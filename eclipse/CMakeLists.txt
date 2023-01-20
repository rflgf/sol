# eclipse #
project(eclipse CXX)

set(eclipse_src
	main.cpp

    DockspaceLayer.cpp
    ImGuiUtils.cpp
    Panels.cpp
)

list(TRANSFORM eclipse_src PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/src/")

add_executable(eclipse ${eclipse_src})

target_link_libraries(eclipse sol)

target_include_directories (eclipse PRIVATE "${CMAKE_SOURCE_DIR}/sol" "${CMAKE_CURRENT_SOURCE_DIR}/src")

set_target_properties(eclipse PROPERTIES
                    ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug/eclipse
                    ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release/eclipse
                    ARCHIVE_OUTPUT_DIRECTORY_DIST ${CMAKE_SOURCE_DIR}/bin/Dist/eclipse
                    LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug/eclipse
                    LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release/eclipse
                    LIBRARY_OUTPUT_DIRECTORY_DIST ${CMAKE_SOURCE_DIR}/bin/Dist/eclipse
                    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug/eclipse
                    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release/eclipse
                    RUNTIME_OUTPUT_DIRECTORY_DIST ${CMAKE_SOURCE_DIR}/bin/Dist/eclipse
)

target_compile_definitions(eclipse PRIVATE
                            _CRT_SECURE_NO_WARNINGS
                            $<$<CONFIG:Debug>:ECL_DEBUG>
                            $<$<CONFIG:Release>:ECL_RELEASE>
                            $<$<CONFIG:Dist>:ECL_DIST>
)
# /eclipse #
