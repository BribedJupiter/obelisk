# Base config
cmake_minimum_required(VERSION 3.30)
project(obelisk)

# Compiler options
add_link_options(-lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Libraries
find_package(OpenGL REQUIRED)
add_subdirectory(lib//glfw-3.4)
include_directories(lib/glad/include)

# Src files
set(SOURCE_FILES src/main.cpp src/glad.c)

# Add executable
add_executable(obelisk ${SOURCE_FILES})
target_link_libraries(obelisk OpenGL::GL glfw3)
