cmake_minimum_required(VERSION 3.15)
project(ManimEngine)

## Compiler settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

## OpenGL
find_package(OpenGL REQUIRED)

## Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets WebEngineWidgets)
qt_standard_project_setup()

## Windows somehow puts build artifacts in "out/". Force Windows to put in build.
if (NOT ${ManimEngine_BUILD_DIR})
    set(ManimEngine_BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
endif()

if (WIN32)
    set (CMAKE_BINARY_DIR ${ManimEngine_BUILD_DIR})
endif()

## Avoid creating cmake_install.cmake
## TODO remove this
# set(CMAKE_SKIP_INSTALL_RULES True)

## Output directories
if (NOT ${ManimEngine_LIB_DIR})
    set(ManimEngine_LIB_DIR "lib")
endif()

if (NOT ${ManimEngine_BIN_DIR})
    set(ManimEngine_LIB_DIR "bin")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${ManimEngine_LIB_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${ManimEngine_LIB_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${ManimEngine_BIN_DIR})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

## Default to debug type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

## Test is disabled by default
if (NOT ${BUILD_TESTING})
    # Disable testing
    message("-- Disabling doctest")
    add_compile_definitions(DOCTEST_CONFIG_DISABLE)

    # Disable asserts
    add_compile_definitions(NDEBUG)
endif()

## Disable optimisation by default
if (${ManimEngine_ENABLE_OPTIMISATION})
    # Enable optimisations
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

## Doctest uses some macros we already use, like INFO and WARN
add_compile_definitions(DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES)


## GLFW options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_VULKAN_STATIC OFF CACHE BOOL "" FORCE)

## Depdendencies
add_subdirectory(external)

## stb
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