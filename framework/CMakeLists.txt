project(Framework)

add_library(${PROJECT_NAME}
    pch.h
    include/Framework.h
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_precompile_headers(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/pch.h
)

add_subdirectory(Core)
add_subdirectory(GeometricTools)
add_subdirectory(Rendering)
add_subdirectory(ECS)
add_subdirectory(Physics)
add_subdirectory(ResourceManagement)

target_link_libraries(${PROJECT_NAME} PUBLIC
    glfw
    glad
    glm
    stb
    spdlog

    FRAMEWORK_CORE
    FRAMEWORK_GEOMETRICTOOLS
    FRAMEWORK_RENDERING
    FRAMEWORK_ECS
    FRAMEWORK_RESOURCE_MANAGEMENT
)
