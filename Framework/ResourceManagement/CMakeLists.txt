project(FRAMEWORK_RESOURCE_MANAGEMENT)

add_library(${PROJECT_NAME}
    JSONParser.cpp
    ShaderManager.cpp
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
    FRAMEWORK_UTIL
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    CONFIG_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config/"
)

# Copy assets directory to binary destination
set (source "${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/")
set (destination "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/shaders/")
add_custom_target (${PROJECT_NAME}_COPY_ASSETS
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${source} ${destination}
)
add_dependencies (${PROJECT_NAME} ${PROJECT_NAME}_COPY_ASSETS)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    SHADERS_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/shaders/"
)