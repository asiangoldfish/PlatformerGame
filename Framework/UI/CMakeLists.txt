project(FRAMEWORK_UI)

add_library(${PROJECT_NAME} STATIC
    UIRoot.cpp
    Box.cpp
    ProgressBar.cpp
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    FRAMEWORK_ECS
)
