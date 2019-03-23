cmake_minimum_required(VERSION 3.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(SRC
    uglobalhotkey/hotkeymap.h
    uglobalhotkey/uglobal.h
    uglobalhotkey/uglobalhotkeys.h
    uglobalhotkey/uglobalhotkeys.cpp
    uglobalhotkey/ukeysequence.h
    uglobalhotkey/ukeysequence.cpp
)

include_directories(
    ${Qt5Widgets_INCLUDE_DIRS}
    ${QtQml_INCLUDE_DIRS}
)

add_library(uglobalhotkey STATIC ${SRC})

if(WIN32)
    target_link_libraries(uglobalhotkey user32)
else()
    target_link_libraries(uglobalhotkey xcb xcb-keysyms)
endif()

set_property(TARGET uglobalhotkey PROPERTY CXX_STANDARD 11)
target_compile_definitions(uglobalhotkey PUBLIC UGLOBALHOTKEY_NOEXPORT)
target_link_libraries(uglobalhotkey Qt5::Core Qt5::Gui Qt5::Widgets)
