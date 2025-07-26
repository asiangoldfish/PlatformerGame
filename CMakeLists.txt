cmake_minimum_required(VERSION 3.15)
project(PlatformerGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)

if (WIN32)
    set (CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
endif()

# Avoid creating cmake_install.cmake
set(CMAKE_SKIP_INSTALL_RULES True)

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
add_subdirectory(External/glad)
add_subdirectory(External/glfw)
add_subdirectory(External/glm)
add_subdirectory(External/spdlog)
#add_subdirectory(External/assimp)
add_subdirectory(External/imgui)
add_subdirectory(External/nlohmann_json)
add_subdirectory(External/doctest)

# stb
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE External/stb)
target_compile_definitions(stb INTERFACE STB_IMAGE_IMPLEMENTATION)
target_compile_definitions(stb INTERFACE STB_IMAGE_STATIC)

add_subdirectory(PhysicsDemo)

# External framework that I contributed to
add_subdirectory(Framework)

# Sample projects
option(BUILD_EXAMPLE "Build example applications" OFF)
if(BUILD_EXAMPLE)
    message(STATUS "Building example projects...")
    add_subdirectory(Example)
endif()