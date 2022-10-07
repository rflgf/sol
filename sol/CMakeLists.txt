cmake_minimum_required(VERSION 3.20)

#sol #
    project(sol CXX)

        set(sol_src "Application.cpp"
                    "EntryPoint.h")

            list(TRANSFORM sol_src PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/src/")

                add_library(sol STATIC $ {sol_src})

#target_link_libraries(sol nlohmann_json imgui imgui - node -                  \
                       editor stb nativefiledialog fmt)

                    target_include_directories(
                        sol PRIVATE $ {CMAKE_CURRENT_SOURCE_DIR} /
                        src PUBLIC $ {CMAKE_CURRENT_SOURCE_DIR})
#/ sol #