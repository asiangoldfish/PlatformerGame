project(FRAMEWORK_RESOURCE_MANAGEMENT)

add_library(${PROJECT_NAME}
    JSONParser.cpp
)

target_include_directories(${PROJECT_NAME}
PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glm
    glfw
    stb

    nlohmann_json

    FRAMEWORK_RENDERING
    FRAMEWORK_CORE
    FRAMEWORK_UTIL
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    CONFIG_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config/"
)