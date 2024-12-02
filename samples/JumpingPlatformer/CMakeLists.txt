project(SamplesJumpingPlatformer)

add_executable(${PROJECT_NAME}
    JumpingPlatformerApp.cpp
    JumpingPlatformerScene.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Framework
)