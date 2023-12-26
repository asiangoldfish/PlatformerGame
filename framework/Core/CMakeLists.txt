project(FRAMEWORK_CORE)

add_library(${PROJECT_NAME} STATIC
    Memory.h
    GLFWApplication.h
    GLFWApplication.cpp
    assertions.h            assertions.cpp
    Timer.h                 Timer.cpp
    EntryPoint.h            EntryPoint.cpp
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}

PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    glfw
    glad
    glm

    FRAMEWORK_RENDERING

PUBLIC
    spdlog
)
