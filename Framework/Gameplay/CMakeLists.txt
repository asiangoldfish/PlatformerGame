project(FRAMEWORK_GAMEPLAY)

add_library(${PROJECT_NAME}
    PlayerController.h      PlayerController.cpp
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}

PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    FRAMEWORK_ECS
)
