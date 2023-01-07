cmake_minimum_required(VERSION 3.5)

project(simplechess-qtclient VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/mainwindow.ui
        src/BoardScene.h
        src/BoardScene.cpp
        src/PieceGraphicsItem.h
        src/PieceGraphicsItem.cpp
        src/SquareGraphicsItem.h
        src/SquareGraphicsItem.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(simplechess-qtclient
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    add_executable(simplechess-qtclient
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(simplechess-qtclient PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
        PRIVATE Qt${QT_VERSION_MAJOR}::Network
	PUBLIC ${CMAKE_SOURCE_DIR}/libs/simple-chess-games/bin/libsimple-chess-games.a)

include_directories(libs/simple-chess-games/include)

set_target_properties(simplechess-qtclient PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS simplechess-qtclient
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(simplechess-qtclient)
endif()
