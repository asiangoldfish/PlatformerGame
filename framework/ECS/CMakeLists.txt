project(FRAMEWORK_ECS)

add_library(${PROJECT_NAME} STATIC
    Entity.h            Entity.cpp
    PrimitiveShape.h    PrimitiveShape.cpp
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}

PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

###############
## Libraries ##
###############

target_link_libraries(${PROJECT_NAME} PUBLIC
    # External
    glm
    glad
    spdlog

    # Internal
    FRAMEWORK_GEOMETRICTOOLS
    FRAMEWORK_RENDERING
    FRAMEWORK_CORE
    FRAMEWORK_PHYSICS
)
