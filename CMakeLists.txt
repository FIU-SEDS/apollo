set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(APPLE)
  set(CMAKE_OSX_ARCHITECTURES "arm64")
endif()

cmake_minimum_required(VERSION 3.15)
project(apollo)
set(CMAKE_CXX_STANDARD 17)

# find libraries
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# set GLAD files
set(GLAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/glad)
set(GLAD_SOURCES ${GLAD_DIR}/src/glad.c)

# set imgui files
# ImGui files
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)


# Your source files
file(GLOB GUI_SOURCES "apollo/gui/*.cpp")
file(GLOB CORE_SOURCES "apollo/core/*.cpp")

set(APOLLO_SOURCES
    ${GUI_SOURCES}
    ${CORE_SOURCES}
)
# Create executable
add_executable(apollo ${APOLLO_SOURCES} ${IMGUI_SOURCES} ${GLAD_SOURCES})

# TODO: add target compile options for compile flags
# TODO: add subdirectories

# include directories
target_include_directories(apollo PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/apollo/
  ${GLAD_DIR}/include
  ${IMGUI_DIR}
  ${IMGUI_DIR}/backends
  /usr/local/Cellar/glfw/3.4/include
)

# link libraries
target_link_libraries(apollo glfw OpenGL::GL)

# macOS specific frameworks
if(APPLE)
    target_link_libraries(apollo 
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()
# instructions:
# run:
# cmake -B build
# link it (in project root):
# ln -s build/compile_commands.json .
