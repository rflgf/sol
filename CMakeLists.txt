cmake_minimum_required(VERSION 3.20)

project(sol_engine)

# set(CMAKE_CXX_COMPILER "clang")
set(CMAKE_CXX_STANDARD "23")
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # dependencies #
# set(vendor_dir "${CMAKE_SOURCE_DIR}/vendor")

# add_subdirectory("${vendor_dir}/fmt")
# add_subdirectory("${vendor_dir}/nlohmann_json")
# add_subdirectory("${vendor_dir}/glad")
# add_subdirectory("${vendor_dir}/SDL")
# project(nativefiledialog LANGUAGES "C")
# add_subdirectory("${vendor_dir}/nativefiledialog")


# #### imgui-node-editor ####
# set(imgui-node-editor_dir "${vendor_dir}/imgui-node-editor")
# add_library(imgui-node-editor STATIC
#     "${imgui-node-editor_dir}/crude_json.cpp"
#     "${imgui-node-editor_dir}/crude_json.h"
#     "${imgui-node-editor_dir}/imgui_bezier_math.h"
#     "${imgui-node-editor_dir}/imgui_bezier_math.inl"
#     "${imgui-node-editor_dir}/imgui_canvas.cpp"
#     "${imgui-node-editor_dir}/imgui_canvas.cpp"
#     "${imgui-node-editor_dir}/imgui_canvas.h"
#     "${imgui-node-editor_dir}/imgui_canvas.h"
#     "${imgui-node-editor_dir}/imgui_extra_math.h"
#     "${imgui-node-editor_dir}/imgui_extra_math.inl"
#     "${imgui-node-editor_dir}/imgui_node_editor_api.cpp"
#     "${imgui-node-editor_dir}/imgui_node_editor_internal.h"
#     "${imgui-node-editor_dir}/imgui_node_editor_internal.inl"
#     "${imgui-node-editor_dir}/imgui_node_editor.cpp"
#     "${imgui-node-editor_dir}/imgui_node_editor.h"
#     "${imgui-node-editor_dir}/misc/imgui_node_editor.natvis"
#     # "${imgui-node-editor_dir}/examples/blueprints-example/utilities/builders.cpp"
#     # "${imgui-node-editor_dir}/examples/blueprints-example/utilities/drawing.cpp"
#     # "${imgui-node-editor_dir}/examples/blueprints-example/utilities/widgets.cpp"
# )
# target_include_directories(imgui-node-editor
#     PUBLIC
#         "${imgui-node-editor_dir}"
#         # "${imgui-node-editor_dir}/examples/blueprints-example"
# )
# target_link_libraries(imgui-node-editor
#     PUBLIC
#         imgui
# )
# #### /imgui-node-editor ####

# #### stb ####
# set(stb_dir "${vendor_dir}/stb")
# add_library(stb STATIC
#     "${stb_dir}/deprecated/stb_image.c"
# )
# set_target_properties(stb PROPERTIES LINKER_LANGUAGE C)
# target_include_directories(stb
#     PUBLIC
#         "${stb_dir}"
# )
# #### /stb ####


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

add_subdirectory(vendor/glm)

add_subdirectory(sol)
add_subdirectory(eclipse)
add_subdirectory(example_app)

set_property(GLOBAL PROPERTY USE_FOLDERS YES)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "Utilities/cmake")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "eclipse")