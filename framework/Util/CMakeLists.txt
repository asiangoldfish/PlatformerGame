project(FRAMEWORK_UTIL)

add_library(${PROJECT_NAME}
    FW_Math.cpp
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}

PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(${PROJECT_NAME} 
PUBLIC
    glm
    spdlog

PRIVATE
    doctest
)
