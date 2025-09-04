project(FRAMEWORK_UTIL)

add_library(${PROJECT_NAME}
    Files.cpp
    Util.cpp
    Math/Math.cpp
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

if(BUILD_TESTING)
    message(STATUS "Building Framework::Util test")
    add_subdirectory(test)
endif()