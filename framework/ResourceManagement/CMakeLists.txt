project(FRAMEWORK_RESOURCE_MANAGEMENT)

add_library(${PROJECT_NAME}
    JSONParser.cpp
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    glm
    glfw
    stb

    nlohmann_json

    FRAMEWORK_RENDERING
)
