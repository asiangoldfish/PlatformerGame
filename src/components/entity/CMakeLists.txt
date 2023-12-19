project(COMPONENT_entity)

add_library(${PROJECT_NAME} STATIC
    Entity.h Entity.cpp
    Floor.h Floor.cpp
    Physics.h Physics.cpp
    Material.h
    Material.cpp
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

    # Internal
    GeometricTools
    Rendering

)
