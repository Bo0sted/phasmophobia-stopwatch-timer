cmake_minimum_required(VERSION 3.16)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(CrossPlatformStopwatch VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Concurrent)
find_package(Qt6 REQUIRED COMPONENTS Network)

include(FetchContent)

FetchContent_Declare(
  QHotkey
  GIT_REPOSITORY https://github.com/Skycoder42/QHotkey.git
  GIT_TAG master
)

FetchContent_MakeAvailable(QHotkey)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CrossPlatformStopwatch
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        qsettingsmanager.h qsettingsmanager.cpp

        stopwatchmanager.h stopwatchmanager.cpp

        stylesheetgenerator.h stylesheetgenerator.cpp
        stopwatchinteractiveeditor.h stopwatchinteractiveeditor.cpp stopwatchinteractiveeditor.ui
        systemtimemodule.h systemtimemodule.cpp systemtimemodule.ui
        colorpickerdialog.h colorpickerdialog.cpp colorpickerdialog.ui
        assets.qrc
        warmwelcome.h warmwelcome.cpp warmwelcome.ui
        version/version
        updatemanager.h updatemanager.cpp
        uioeventloop.h uioeventloop.cpp
        uiohotkeymanager.h uiohotkeymanager.cpp
        uiokey.h
        gradientqlabel.h gradientqlabel.cpp



    )

target_include_directories(CrossPlatformStopwatch
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET CrossPlatformStopwatch APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(CrossPlatformStopwatch SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(CrossPlatformStopwatch
            ${PROJECT_SOURCES}
        )
    endif()
endif()

if(WIN32)
    set(ICON_PATH "${CMAKE_CURRENT_SOURCE_DIR}/assets/win.ico")
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/appicon.rc.in"
        "${CMAKE_CURRENT_BINARY_DIR}/appicon.rc"
        @ONLY
    )
    target_sources(CrossPlatformStopwatch PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/appicon.rc")
endif()

if(UNIX AND NOT APPLE)
    install(FILES assets/512x512/stopwatchicon.png
            DESTINATION share/icons/hicolor/512x512/apps)

    set(EXEC_PATH "${CMAKE_INSTALL_PREFIX}/bin/CrossPlatformStopwatch")
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/CrossPlatformStopwatch.desktop.in
        ${CMAKE_CURRENT_BINARY_DIR}/CrossPlatformStopwatch.desktop
        @ONLY
    )

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CrossPlatformStopwatch.desktop
            DESTINATION share/applications)
endif()


add_subdirectory(vendor/libuiohook)
target_link_libraries(CrossPlatformStopwatch PRIVATE uiohook)
target_include_directories(CrossPlatformStopwatch PRIVATE vendor/libuiohook/include)
target_link_libraries(CrossPlatformStopwatch PRIVATE Qt${QT_VERSION_MAJOR}::Widgets qhotkey)
target_link_libraries(CrossPlatformStopwatch PRIVATE Qt6::Concurrent)
target_link_libraries(CrossPlatformStopwatch PRIVATE Qt6::Network)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.CrossPlatformStopwatch)
endif()
set_target_properties(CrossPlatformStopwatch PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS CrossPlatformStopwatch
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CrossPlatformStopwatch)
endif()
