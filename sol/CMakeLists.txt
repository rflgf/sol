cmake_minimum_required(VERSION 3.20)

# sol #
project(sol CXX)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/debug-trap)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb)

set(sol_src
    "Application.cpp"
    "CameraController.cpp"
    "Layer.cpp"
    "LayerStack.cpp"
	"Log.cpp"
    "Platform.cpp"
	"Timestep.h"
    "Utils.cpp"
    
    "ImGui/ImGuiLayer.cpp"

    "Platform/OpenGL/OpenGLBuffers.cpp"
    "Platform/OpenGL/OpenGLFramebuffer.cpp"
    "Platform/OpenGL/OpenGLContext.cpp"
    "Platform/OpenGL/OpenGLRendererAPI.cpp"
    "Platform/OpenGL/OpenGLShader.cpp"
    "Platform/OpenGL/OpenGLTexture.cpp"

    "Platform/Windows/WindowsInput.cpp"
    "Platform/Windows/WindowsPlatform.cpp"
    "Platform/Windows/WindowsWindow.cpp"

    "Renderer/Buffers.cpp"
    "Renderer/Framebuffer.cpp"
    "Renderer/OrthographicCamera.cpp"
    "Renderer/RenderCommand.cpp"
    "Renderer/Renderer.cpp"
    "Renderer/Renderer2D.cpp"
    "Renderer/RendererAPI.cpp"
    "Renderer/Shader.cpp"
    "Renderer/Subtexture2D.cpp"
    "Renderer/Texture.cpp"

    "Scene/Scene.cpp"
    "Scene/SceneCamera.cpp"
    "Scene/SceneSerializer.cpp"
)

list(TRANSFORM sol_src PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/src/")

add_library(sol STATIC ${sol_src})

target_link_libraries(sol debug-trap glad glm EnTT imgui ImGuizmo nfd SDL2-static SDL2main spdlog stb_image yaml-cpp)

target_include_directories(sol
PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}
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
