cmake_minimum_required(VERSION 3.15)
project(apollo)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

if(APPLE)
  set(CMAKE_OSX_ARCHITECTURES "arm64")
endif()

# find libraries
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# set GLAD files
set(GLAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/glad)
set(GLAD_SOURCES ${GLAD_DIR}/src/glad.c)

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

# Include directories
target_include_directories(apollo PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/apollo/
    ${GLAD_DIR}/include
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# Link libraries
target_link_libraries(apollo glfw OpenGL::GL)

# Platform-specific libraries
if(APPLE)
    target_link_libraries(apollo
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
elseif(UNIX)
    target_link_libraries(apollo dl)
elseif(WIN32)
    target_link_libraries(apollo opengl32)
endif()