project(FRAMEWORK_UTIL)

add_library(${PROJECT_NAME}
    FW_Math.h       FW_Math.cpp
    Util.h
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}

PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(${PROJECT_NAME}
PUBLIC
    FRAMEWORK_CORE

PRIVATE
    glm
)
