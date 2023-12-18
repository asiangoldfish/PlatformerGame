set(MODULE_NAME GLFWApplication)

add_library(${MODULE_NAME} STATIC
    GLFWApplication.h
    GLFWApplication.cpp
    assertions.h            assertions.cpp
    Timer.h                 Timer.cpp
)

target_include_directories(${MODULE_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${MODULE_NAME} PRIVATE 
    glfw
    glad
    glm
)
