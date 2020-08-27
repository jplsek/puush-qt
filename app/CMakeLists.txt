set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
cmake_policy(SET CMP0071 NEW)

find_package(Qt5 COMPONENTS Core Gui Qml Widgets QuickControls2 QuickCompiler REQUIRED)

qtquick_compiler_add_resources(RC_SRC
    ../resources.qrc
)

set(SRC
    main.cpp
    systray.h
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

# Install and CPack

set(CPACK_PACKAGE_VERSION "0.2.5")
set(CPACK_PACKAGE_VENDOR "Jeremy Plsek")
set(CPACK_PACKAGE_CONTACT "Jeremy Plsek")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A GUI frontend for puush.")
set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/../resources/images/puush-qt.svg")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/../README.md")
set(CPACK_PACKAGE_EXECUTABLES "puush-qt;puush-qt")

if(WIN32)
    # Get OpenSSL
    set(OSSLZIP ${CMAKE_CURRENT_BINARY_DIR}/openssl.zip)
    set(OSSLDLL ${CMAKE_CURRENT_BINARY_DIR}/ssleay32.dll)

    if(NOT EXISTS ${OSSLDLL})
        if(NOT EXISTS ${OSSLZIP})
            file(DOWNLOAD https://indy.fulgan.com/SSL/Archive/openssl-1.0.2p-x64_86-win64.zip ${OSSLZIP}
                TIMEOUT 15
                EXPECTED_HASH MD5=6c523fc530a97553d9316a51008266e7
                TLS_VERIFY ON
            )
        endif()

        execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xf ${OSSLZIP}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    endif()

    # Create install
    install(TARGETS puush-qt DESTINATION .)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ssleay32.dll DESTINATION .)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libeay32.dll DESTINATION .)

    # Add libraries
    include(windeployqt.cmake)
    windeployqt(puush-qt .)

    set(CPACK_PACKAGE_INSTALL_DIRECTORY "puush-qt")
    set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/..\\\\resources\\\\images\\\\puush-qt.svg")
    set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/puush-qt.ico")
    set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/puush-qt.ico")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    set(CPACK_NSIS_INSTALLED_ICON_NAME "puush-qt.exe")
    set(CPACK_NSIS_EXECUTABLES_DIRECTORY ".")
    set(CPACK_GENERATOR "ZIP" "NSIS")
else()
    install(TARGETS puush-qt DESTINATION bin)
    install(FILES puush-qt.desktop DESTINATION share/applications)
    install(FILES ../resources/images/puush-qt.svg DESTINATION share/icons/hicolor/scalable/apps)

    option(MakeDeb "DEB" OFF)
    option(MakeRPM "RPM" OFF)

    set(generators "TGZ" "STGZ")
    if(MakeDeb)
        list(APPEND generators "DEB")
    endif()
    if(MakeRPM)
        list(APPEND generators "RPM")
    endif()
    set(CPACK_GENERATOR ${generators})
endif()

include(CPack)
