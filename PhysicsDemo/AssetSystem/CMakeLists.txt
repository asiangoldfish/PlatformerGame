project(PhysicsDemo_AssetSystem)

add_library(${PROJECT_NAME}
    Asset.cpp
    AssetSystem.cpp
    Image.cpp
)

target_include_directories(${PROJECT_NAME} 
PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    Framework)