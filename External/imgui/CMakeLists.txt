project(imgui)

file(GLOB IMGUI_SOURCE_FILES
    "*.cpp"
    "*.h"
    imgui.h
)

add_compile_definitions(
    IMGUI_IMPL_OPENGL_LOADER_CUSTOM
)

add_library(imgui STATIC ${IMGUI_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    glad
    glfw
)

target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
