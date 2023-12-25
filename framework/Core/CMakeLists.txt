set(MODULE_NAME Core)

add_library(${MODULE_NAME} STATIC
    Core.h
    GLFWApplication.h
    GLFWApplication.cpp
    assertions.h            assertions.cpp
    Timer.h                 Timer.cpp
)

target_include_directories(${MODULE_NAME}
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}

PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(${MODULE_NAME} PRIVATE
    glfw
    glad
    glm
    spdlog

    Rendering
)
