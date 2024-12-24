project(SamplesJumpingPlatformer)

add_executable(${PROJECT_NAME}
    App.cpp
    Scene.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Framework
    # Uncomment below to link to doctest and test the project.
    # doctest
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    TEXTURES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/"
    RESOURCES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/"
    CONFIG_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config/"
)