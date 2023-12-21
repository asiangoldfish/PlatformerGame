set(MODULE_NAME Rendering)

add_library(${MODULE_NAME} STATIC
    VertexBuffer.h          VertexBuffer.cpp
    IndexBuffer.h           IndexBuffer.cpp
    BufferLayout.h
    ShaderDataTypes.h
    VertexArray.h           VertexArray.cpp
    Shader.h                Shader.cpp
    Camera.h                Camera.cpp
    RenderCommands.h
    Texture.h               Texture.cpp
    TextureManager.h        TextureManager.cpp
    OrthographicCamera.h    OrthographicCamera.cpp
    PerspectiveCamera.h     PerspectiveCamera.cpp
    Material.h              Material.cpp

    # Lighting
    Light/Light.h
    Light/DirectionalLight.h    Light/DirectionalLight.cpp
    Light/SpotLight.h           Light/SpotLight.cpp
    Light/PointLight.h          Light/PointLight.cpp
)

target_include_directories(${MODULE_NAME}
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}

PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(${MODULE_NAME} PUBLIC
    glfw
    glad
    glm
    stb
    spdlog

    Core
)
