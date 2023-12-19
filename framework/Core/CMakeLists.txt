set(MODULE_NAME Core)

add_library(${MODULE_NAME} STATIC
    Log.h
    Log.cpp
)

target_include_directories(${MODULE_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${MODULE_NAME} PUBLIC
    spdlog
)
