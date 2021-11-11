cmake_minimum_required(VERSION 3.10)
project(OpenGL-texture)

# use C++17 (in case std::fs needed)
set(CMAKE_CXX_STANDARD 17)

# autocomplete with YCM & debug with gdb
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_BUILD_TYPE Debug)

# needed packages
find_package(glfw3 REQUIRED)

# shared library
file(GLOB SRC
  "src/*.cpp"
  "src/glad/glad.c"
)
add_library(opengl_texture SHARED ${SRC})
target_include_directories(opengl_texture PRIVATE include)
target_link_libraries(opengl_texture glfw)
