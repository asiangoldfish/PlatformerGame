project(Example_SpaceExplorer)

add_executable(${PROJECT_NAME}
    App.cpp
    Scene.cpp
    Ship.cpp
    GameUI.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Framework
)
