cmake_minimum_required(VERSION 3.5)

project(hydrogen-windows-ui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS QuickWidgets Core Gui WebChannel WebEngineWidgets)

set(PROJECT_SOURCES
        main.cpp

        core/MainWindow.cpp
        core/MainWindow.h
        core/MainWindow.ui

        widgets/Dashboard.h
        widgets/Dashboard.cpp
        widgets/Dashboard.ui
        widgets/Executor.h
        widgets/Executor.cpp
        widgets/Executor.ui

        dialogs/AboutDialog.h
        dialogs/AboutDialog.cpp
        dialogs/AboutDialog.ui
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/monaco
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(hydrogen-windows-ui
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET hydrogen-windows-ui APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(hydrogen-windows-ui SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(hydrogen-windows-ui
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(hydrogen-windows-ui PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::QuickWidgets Qt6::WebChannel Qt6::WebEngineWidgets)

set(hydrogen_qml_files
    qml/test.qml
)

qt_add_qml_module(hydrogen-windows-ui
    URI QuickWidgetExample
    VERSION 1.0
    QML_FILES
        ${hydrogen_qml_files}
    RESOURCE_PREFIX /hydrogen
    NO_RESOURCE_TARGET_PATH
)

set_target_properties(hydrogen-windows-ui PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS hydrogen-windows-ui
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(hydrogen-windows-ui)
endif()
