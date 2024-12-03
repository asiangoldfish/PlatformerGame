project(SamplesJumpingPlatformer)

add_executable(${PROJECT_NAME}
    JumpingPlatformerApp.cpp
    JumpingPlatformerScene.cpp
    Sprite.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Framework
)

file(
    COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources" 
    DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources")