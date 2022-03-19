cmake_minimum_required(VERSION 3.10)
project(OpenGL-framebuffer)

# use C++17 (in case std::fs needed)
set(CMAKE_CXX_STANDARD 17)

# autocomplete with YCM & debug with gdb
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_BUILD_TYPE Debug)

# submodules
add_subdirectory(opengl-texture)

# shared library
file(GLOB SRC "src/*.cpp")
add_library(opengl_framebuffer SHARED ${SRC})
target_include_directories(opengl_framebuffer PUBLIC include)
target_link_libraries(opengl_framebuffer opengl_texture)
