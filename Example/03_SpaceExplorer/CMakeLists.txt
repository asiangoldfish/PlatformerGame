project(Example_SpaceExplorer)

add_executable(${PROJECT_NAME}
    App.cpp
    Scene.cpp
    Ship.cpp
    GameUI.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Framework
)
