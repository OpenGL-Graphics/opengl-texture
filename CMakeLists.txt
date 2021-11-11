cmake_minimum_required(VERSION 3.10)
project(OpenGL)

# use C++17 (in case std::fs needed)
set(CMAKE_CXX_STANDARD 17)

# autocomplete with YCM & debug with gdb
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_BUILD_TYPE Debug)

# shared library
add_library(opengl SHARED "src/glad/glad.c")
target_include_directories(opengl PUBLIC include)
