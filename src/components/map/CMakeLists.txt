project(COMPONENT_map)

add_library(${PROJECT_NAME} Map.h Map.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    COMPONENT_entity
    Framework
)
