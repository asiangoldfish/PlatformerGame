project(PhysicsDemo)

add_executable(${PROJECT_NAME}
    PhysicsApp.cpp
    Widgets/ImGuiWidgets.hpp
    Widgets/SettingsWidget.hpp
    Widgets/MenuBarWidget.hpp
)

add_subdirectory(components)

target_link_libraries(${PROJECT_NAME}
    imgui

    # Framework
    Framework
    COMPONENT_entity2
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    TEXTURES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/"
    RESOURCES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/"
    CONFIG_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config/"
)

# Copy assets directory to binary destination
set (source "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set (destination "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources")
add_custom_target (${PROJECT_NAME}_COPY_ASSETS
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${source} ${destination}
)
add_dependencies (${PROJECT_NAME} ${PROJECT_NAME}_COPY_ASSETS)

# Copy assets directory to binary destination
set (source "${CMAKE_CURRENT_SOURCE_DIR}/config")
set (destination "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config")
add_custom_target (${PROJECT_NAME}_COPY_CONFIG
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${source} ${destination}
)
add_dependencies (${PROJECT_NAME} ${PROJECT_NAME}_COPY_CONFIG)
