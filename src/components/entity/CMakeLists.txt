project(COMPONENT_entity)

add_library(${PROJECT_NAME} STATIC
    Cube.h
    Floor.h     Floor.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

###############
## Libraries ##
###############

target_link_libraries(${PROJECT_NAME} PUBLIC
    # External
    glfw
    glad
    glm
    stb
    spdlog

    # Internal
    Framework
)
