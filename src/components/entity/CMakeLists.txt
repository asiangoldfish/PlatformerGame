project(COMPONENT_entity)

add_library(${PROJECT_NAME} STATIC
    Cube.h
    Floor.h Floor.cpp
    Entity.h Entity.cpp
    Physics.h Physics.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

###############
## Libraries ##
###############

# GLFW
target_link_libraries(${PROJECT_NAME} PRIVATE
    # Internal
    GeometricTools
    Rendering

    # External
    glfw
    glad
    glm
    stb
)
