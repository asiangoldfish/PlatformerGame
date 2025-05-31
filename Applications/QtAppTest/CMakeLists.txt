project(QtAppTest)

qt_add_executable(${PROJECT_NAME}
    main.cpp
    QtApp.cpp
    WebWindow.cpp
    VSCodiumManager.cpp
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    Qt6::WebEngineWidgets
    Qt6::Widgets
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)