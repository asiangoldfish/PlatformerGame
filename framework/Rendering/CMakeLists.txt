set(MODULE_NAME Rendering)

add_library(${MODULE_NAME} STATIC
    # Buffers
    Buffers/VertexBuffer.h  Buffers/VertexBuffer.cpp
    Buffers/IndexBuffer.h   Buffers/IndexBuffer.cpp
    Buffers/BufferLayout.h
    Buffers/VertexArray.h   Buffers/VertexArray.cpp
    Buffers/Framebuffer.h   Buffers/Framebuffer.cpp

    # Camera
    Camera/Camera.h Camera/Camera.cpp
    Camera/OrthographicCamera.h Camera/OrthographicCamera.cpp
    Camera/PerspectiveCamera.h Camera/PerspectiveCamera.cpp

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

    # Miscellaneous
    RenderCommands.h
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
