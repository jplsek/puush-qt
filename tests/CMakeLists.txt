set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Test Widgets REQUIRED)

set(SRC
    main.cpp
    main.h
)

include_directories(
    ../lib/uglobalhotkey
    ../src
    ${Qt5Widgets_INCLUDE_DIRS}
    ${QtQml_INCLUDE_DIRS}
)

if(WIN32)
    add_executable(puush-qt-test WIN32 ${SRC})
else()
    add_executable(puush-qt-test ${SRC})
endif()

set_property(TARGET puush-qt-test PROPERTY CXX_STANDARD 11)
target_link_libraries(puush-qt-test Qt5::Core Qt5::Test Qt5::Widgets puush-qt-lib uglobalhotkey)
