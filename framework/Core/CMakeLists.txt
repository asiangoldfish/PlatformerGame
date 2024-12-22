project(FRAMEWORK_CORE)

add_library(${PROJECT_NAME} STATIC
    GLFWApplication.cpp
    Timer.cpp
    EntryPoint.cpp
    Input.cpp
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}

#PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glad
    glm
    spdlog
    doctest

    FRAMEWORK_RENDERING
    FRAMEWORK_PHYSICS
    FRAMEWORK_UTIL
)
