project(Example_SpaceExplorer)

add_executable(${PROJECT_NAME}
    App.cpp
    Scene.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Framework
)
