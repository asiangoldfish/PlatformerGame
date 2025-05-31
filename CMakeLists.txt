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

# Default to debug type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE MATCHES Release)
    # Disable testing
    message("-- Disabling doctest")
    add_compile_definitions(DOCTEST_CONFIG_DISABLE)

    # Enable optimisations
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")

    # Disable asserts
    add_compile_definitions(NDEBUG)
endif(CMAKE_BUILD_TYPE MATCHES Release)

if(DISABLE_TEST MATCHES true)
    # Testing is disabled with -DDISABLE_TESTING flag
    message("-- Disabling doctest")
    add_compile_definitions(DOCTEST_CONFIG_DISABLE)
endif()

# Doctest uses some macros we already use, like INFO and WARN
add_compile_definitions(DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES)



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
#add_subdirectory(external/assimp)
add_subdirectory(external/imgui)
add_subdirectory(external/nlohmann_json)
add_subdirectory(external/doctest)

# stb
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE external/stb)
target_compile_definitions(stb INTERFACE STB_IMAGE_IMPLEMENTATION)
target_compile_definitions(stb INTERFACE STB_IMAGE_STATIC)

add_subdirectory(Applications)

# TODO fix this repo and move to samples
# add_subdirectory(src)
add_subdirectory(PhysicsDemo)

# External framework that I contributed to
add_subdirectory(framework)

# Sample projects
add_subdirectory(samples)