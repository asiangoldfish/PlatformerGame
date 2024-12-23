project(FRAMEWORK_PHYSICS)

add_library(${PROJECT_NAME} STATIC
    BoundingBoxes.cpp
    ParticleSystem.cpp
    PhysicsEngine.cpp
    Force.cpp
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}

PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glm
    spdlog

    FRAMEWORK_RENDERING
    FRAMEWORK_GEOMETRICTOOLS
    FRAMEWORK_UTIL
)



target_compile_definitions(${PROJECT_NAME} PRIVATE
    FW_PHYSICS_RESOURCES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/"
)

# Copy assets directory to binary destination
set (source "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set (destination "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources")
add_custom_target (${PROJECT_NAME}_COPY_ASSETS
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${source} ${destination}
)
add_dependencies (${PROJECT_NAME} ${PROJECT_NAME}_COPY_ASSETS)
