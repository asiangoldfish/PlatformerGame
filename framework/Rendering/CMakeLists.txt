project(FRAMEWORK_RENDERING)

add_library(${PROJECT_NAME} STATIC
    Buffer.h        Buffer.cpp

    # Camera
    Camera/Camera.h                 Camera/Camera.cpp
    Camera/OrthographicCamera.h     Camera/OrthographicCamera.cpp
    Camera/PerspectiveCamera.h      Camera/PerspectiveCamera.cpp
    Camera/CameraController.h       Camera/CameraController.cpp

    # Shading
    ShaderDataTypes.h
    Shader.h                Shader.cpp
    Texture.h               Texture.cpp
    TextureManager.h        TextureManager.cpp
    Material.h              Material.cpp

    # Lighting
    Light/Light.h
    Light/DirectionalLight.h    Light/DirectionalLight.cpp
    Light/SpotLight.h           Light/SpotLight.cpp
    Light/PointLight.h          Light/PointLight.cpp
    Light/Skybox.h              Light/Skybox.cpp

    # Miscellaneous
    RenderCommands.h
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}

PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    glfw
    glad
    glm
    stb
    spdlog

    FRAMEWORK_CORE
    FRAMEWORK_GEOMETRICTOOLS
)
