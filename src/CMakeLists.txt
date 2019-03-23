set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Network Widgets REQUIRED)

set(SRC
    general.cpp
    general.h
    authentication.cpp
    authentication.h
    history.cpp
    history.h
    information.cpp
    information.h
    screenshot.cpp
    screenshot.h
    upload.cpp
    upload.h
    keybindings.cpp
    keybindings.h
    settings.cpp
    settings.h
    transparentwindow.cpp
    transparentwindow.h
    api/apiauth.cpp
    api/apiauth.h
    api/apidel.cpp
    api/apidel.h
    api/apihist.cpp
    api/apihist.h
    api/apirequest.cpp
    api/apirequest.h
    api/apithumb.cpp
    api/apithumb.h
)

include_directories(
    ../lib/uglobalhotkey
    ${Qt5Widgets_INCLUDE_DIRS}
    ${QtQml_INCLUDE_DIRS}
)

add_library(puush-qt-lib STATIC ${SRC})

set_property(TARGET puush-qt-lib PROPERTY CXX_STANDARD 11)
target_link_libraries(puush-qt-lib uglobalhotkey Qt5::Widgets Qt5::Network)
