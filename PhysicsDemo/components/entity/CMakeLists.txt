project(COMPONENT_entity2)

add_library(${PROJECT_NAME} STATIC
    WorldGrid.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

###############
## Libraries ##
###############

target_link_libraries(${PROJECT_NAME} PRIVATE
    # External
    glfw
    glad
    glm
    stb
    spdlog

    # Internal
    Framework
)
