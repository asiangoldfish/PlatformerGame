project(FRAMEWORK_ECS)

add_library(${PROJECT_NAME} STATIC
    Entity.cpp
    Shape.cpp
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

target_link_libraries(${PROJECT_NAME} 
PUBLIC
    glad

PRIVATE
    # External
    doctest
    glm
    spdlog

    # Internal
    FRAMEWORK_GEOMETRICTOOLS
    FRAMEWORK_RENDERING
    FRAMEWORK_PHYSICS
    FRAMEWORK_UTIL
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    FW_ECS_RESOURCES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/"
)

# Copy assets directory to binary destination
set (source "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set (destination "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources")
add_custom_target (${PROJECT_NAME}_COPY_ASSETS
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${source} ${destination}
)
add_dependencies (${PROJECT_NAME} ${PROJECT_NAME}_COPY_ASSETS)
