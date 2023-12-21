project(ResourceManagement)

add_library(${PROJECT_NAME}
    Model.h     Model.cpp
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    assimp
    glm
    glfw
    stb

    Rendering
)
