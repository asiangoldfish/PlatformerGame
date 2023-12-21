cmake_minimum_required(VERSION 3.15)
project(PlatformerGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)

if (WIN32)
    set (CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# GLFW options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_VULKAN_STATIC OFF CACHE BOOL "" FORCE)

# Depdendencies
add_subdirectory(external/glad)
add_subdirectory(external/glfw)
add_subdirectory(external/glm)
add_subdirectory(external/spdlog)
add_subdirectory(external/assimp)

# stb
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE external/stb)
target_compile_definitions(stb INTERFACE STB_IMAGE_IMPLEMENTATION)
target_compile_definitions(stb INTERFACE STB_IMAGE_STATIC)

add_subdirectory(src)

# External framework that I contributed to
add_subdirectory(framework)
