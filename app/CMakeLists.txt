cmake_minimum_required(VERSION 3.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Gui Qml Widgets QuickControls2 REQUIRED)

qt5_add_resources(RC_SRC
    ../resources.qrc
)

set(SRC
    systray.h
    main.cpp
    systray.cpp
    puush-qt.rc
    ${RC_SRC}
)

include_directories(
    ../lib/uglobalhotkey
    ../src
    ${Qt5Widgets_INCLUDE_DIRS}
    ${QtQml_INCLUDE_DIRS}
)

if(WIN32)
    add_executable(puush-qt WIN32 ${SRC})
else()
    add_executable(puush-qt ${SRC})
endif()

set_property(TARGET puush-qt PROPERTY CXX_STANDARD 11)
target_link_libraries(puush-qt Qt5::Core Qt5::Gui Qt5::Qml Qt5::Widgets Qt5::QuickControls2 puush-qt-lib uglobalhotkey)
