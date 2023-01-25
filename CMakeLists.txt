cmake_minimum_required(VERSION 3.20)

project(sol_engine)

# set(CMAKE_C_COMPILER clang)
# set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD "23")
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

## dependencies ##

#### imgui ####
set(imgui_dir "vendor/imgui")
set(imgui_src
	"${imgui_dir}/imgui.cpp"
	"${imgui_dir}/imgui_demo.cpp"
	"${imgui_dir}/imgui_draw.cpp"
	"${imgui_dir}/imgui_tables.cpp"
	"${imgui_dir}/imgui_widgets.cpp"

	"${imgui_dir}/backends/imgui_impl_sdl.cpp"
	"${imgui_dir}/backends/imgui_impl_sdlrenderer.cpp"
	"${imgui_dir}/backends/imgui_impl_opengl3.cpp"
	"${imgui_dir}/backends/imgui_impl_opengl3_loader.h"

    "${imgui_dir}/misc/cpp/imgui_stdlib.cpp"
)
source_group("" FILES ${imgui_src})
add_library(imgui STATIC ${imgui_src})
target_include_directories(imgui
	PUBLIC
		"${imgui_dir}"
)
target_compile_definitions(imgui
    PUBLIC
        "-D IMGUI_IMPL_OPENGL_LOADER_GLAD"
)
target_link_libraries(imgui SDL2main SDL2-static glad)
#### /imgui ####

add_subdirectory(vendor/entt)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/nativefiledialog-extended)
add_subdirectory(vendor/yaml-cpp)

add_subdirectory(sol)
add_subdirectory(eclipse)
add_subdirectory(example_app)

set_property(GLOBAL PROPERTY USE_FOLDERS YES)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "Utilities/cmake")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "eclipse")