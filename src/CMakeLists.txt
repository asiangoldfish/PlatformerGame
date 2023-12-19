project(MyApplication)

add_executable(${PROJECT_NAME}
    main.cpp
    SokobanApplication.h
    SokobanApplication.cpp
)

add_subdirectory(components)

target_link_libraries(${PROJECT_NAME}
    # External
    glfw
    glad
    glm
    stb

    # Framework
    Core
    GLFWApplication
    GeometricTools
    Rendering

    # Components
    COMPONENT_entity
    COMPONENT_util
    COMPONENT_map
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    TEXTURES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/"
    RESOURCES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/"
)

# Copy assets directory to binary destination
set (source "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set (destination "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources")
add_custom_target (${PROJECT_NAME}_COPY_ASSETS
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${source} ${destination}
)
add_dependencies (${PROJECT_NAME} ${PROJECT_NAME}_COPY_ASSETS)

