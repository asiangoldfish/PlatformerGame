project(FRAMEWORK_PHYSICS)

add_library(${PROJECT_NAME}
    Physics.h   Physics.cpp
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}

PRIVATE
    ${CMAKE_SOURCE_DIR}/framework
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    FRAMEWORK_ECS
    glm
)
