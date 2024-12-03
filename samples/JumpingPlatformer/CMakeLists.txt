project(SamplesJumpingPlatformer)

add_executable(${PROJECT_NAME}
    JumpingPlatformerApp.cpp
    JumpingPlatformerScene.cpp
    Sprite.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Framework
)

# Copy assets directory to binary destination
set (source "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set (destination "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources")
add_custom_target (${PROJECT_NAME}_COPY_ASSETS
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${source} ${destination}
)
add_dependencies (${PROJECT_NAME} ${PROJECT_NAME}_COPY_ASSETS)


target_compile_definitions(${PROJECT_NAME} PRIVATE
    TEXTURES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/"
    RESOURCES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/"
    CONFIG_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config/"
)