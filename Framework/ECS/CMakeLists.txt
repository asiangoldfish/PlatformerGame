project(FRAMEWORK_ECS)

add_library(${PROJECT_NAME} STATIC
    Entity.cpp
    BaseScene.cpp
    Component.cpp
    Sprite.cpp
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}

PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

###############
## Libraries ##
###############

target_link_libraries(${PROJECT_NAME} PUBLIC
    # External
    glm
    glad
    spdlog

    # Internal
    FRAMEWORK_GEOMETRICTOOLS
    FRAMEWORK_RENDERING
    FRAMEWORK_CORE
    FRAMEWORK_PHYSICS
)

# Copy assets directory to binary destination
set (source "${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/")
set (destination "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/shaders/")
add_custom_target (${PROJECT_NAME}_COPY_ASSETS
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${source} ${destination}
)
add_dependencies (${PROJECT_NAME} ${PROJECT_NAME}_COPY_ASSETS)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    SHADERS_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/shaders/"
)