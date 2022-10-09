cmake_minimum_required(VERSION 3.20)

# sol #
project(sol CXX)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog)

set(sol_src
	"Application.cpp"
	"Log.cpp"
)

list(TRANSFORM sol_src PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/src/")

add_library(sol STATIC ${sol_src})

target_link_libraries(sol spdlog)
# target_link_libraries(sol nlohmann_json imgui imgui-node-editor stb nativefiledialog fmt)

target_include_directories(sol
PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)
# /sol #