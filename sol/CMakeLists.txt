cmake_minimum_required(VERSION 3.20)

# sol #
project(sol CXX)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad)

set(sol_src
	"Application.cpp"
    "Input.cpp"
    "Layer.cpp"
    "LayerStack.cpp"
	"Log.cpp"
    "Platform.cpp"
	"Timestep.h"
    
    "ImGui/ImGuiLayer.cpp"

    "Platform/OpenGL/OpenGLBuffers.cpp"
    "Platform/OpenGL/OpenGLContext.cpp"
    "Platform/OpenGL/OpenGLRendererAPI.cpp"
    "Platform/OpenGL/OpenGLShader.cpp"

    "Platform/Windows/WindowsInput.cpp"
    "Platform/Windows/WindowsPlatform.cpp"
    "Platform/Windows/WindowsWindow.cpp"

    "Renderer/Buffers.cpp"
    "Renderer/OrthographicCamera.cpp"
    "Renderer/RenderCommand.cpp"
    "Renderer/Renderer.cpp"
    "Renderer/RendererAPI.cpp"
    "Renderer/Shader.cpp"
)

list(TRANSFORM sol_src PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/src/")

add_library(sol STATIC ${sol_src})

target_link_libraries(sol SDL2-static SDL2main glad glm imgui spdlog)

target_include_directories(sol
PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/debug_trap
)

set_target_properties(sol PROPERTIES
                    ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug/sol
                    ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release/sol
                    ARCHIVE_OUTPUT_DIRECTORY_DIST ${CMAKE_SOURCE_DIR}/bin/Dist/sol
                    LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug/sol
                    LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release/sol
                    LIBRARY_OUTPUT_DIRECTORY_DIST ${CMAKE_SOURCE_DIR}/bin/Dist/sol
                    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug/sol
                    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release/sol
                    RUNTIME_OUTPUT_DIRECTORY_DIST ${CMAKE_SOURCE_DIR}/bin/Dist/sol
)

# target_precompile_headers(sol
#   PRIVATE
#     [[SDL.h]]
# )

target_compile_definitions(sol PRIVATE
                            _CRT_SECURE_NO_WARNINGS
                            GLFW_INCLUDE_NONE
                            YAML_CPP_STATIC_DEFINE
                            $<$<CONFIG:Debug>:SOL_DEBUG>
                            $<$<CONFIG:Release>:SOL_RELEASE>
                            $<$<CONFIG:Dist>:SOL_DIST>
                            
                            SOL_PLATFORM_WINDOWS
)
# /sol #
