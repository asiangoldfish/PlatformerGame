project(PhysicsDemo)

add_executable(${PROJECT_NAME}
    PhysicsApp.cpp
    PhysicsScene.cpp
    Widgets/AppWidget.cpp
    ExitStatus.cpp
    Selections.cpp
)

add_subdirectory(components)

target_include_directories(${PROJECT_NAME} PRIVATE
    Widgets/
)

add_subdirectory(AssetSystem)

# Because Windows does not link automatically to OpenGL, we need to figure out
# whether this is built on a Windows machine. This is used to reuplaod font
# texture to the GPU by AppWidget.
if(WIN32)
    # On Windows, link against opengl32.lib
    find_package(OpenGL REQUIRED)
    set(OpenGlLib opengl32)
else()
    set(OpenGLLib "")
endif()

target_link_libraries(${PROJECT_NAME}
    imgui

    # Framework
    Framework
    COMPONENT_entity2
    PhysicsDemo_AssetSystem
    ${OpenGlLib}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    TEXTURES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/"
    RESOURCES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/"
    CONFIG_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config/"
    TEMPLATES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/templates/"
)

# Copy resources directory to binary destination
set (source "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set (destination "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources")
add_custom_target (${PROJECT_NAME}_COPY_ASSETS
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${source} ${destination}
)
add_dependencies (${PROJECT_NAME} ${PROJECT_NAME}_COPY_ASSETS)

# Copy config directory to binary destination
set (source "${CMAKE_CURRENT_SOURCE_DIR}/config")
set (destination "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config")
add_custom_target (${PROJECT_NAME}_COPY_CONFIG
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${source} ${destination}
)
add_dependencies (${PROJECT_NAME} ${PROJECT_NAME}_COPY_CONFIG)

# Copy templates directory to binary destination
set (source "${CMAKE_CURRENT_SOURCE_DIR}/templates")
set (destination "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/templates")
add_custom_target (${PROJECT_NAME}_TEMPLATES_CONFIG
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${source} ${destination}
)
add_dependencies (${PROJECT_NAME} ${PROJECT_NAME}_TEMPLATES_CONFIG)