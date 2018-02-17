include($$PWD/../puush-qt.pri)

QT += core network widgets

TEMPLATE = lib
TARGET = puush-qt

SOURCES += \
    screenshot.cpp \
    upload.cpp \
    api/apirequest.cpp \
    api/apiauth.cpp \
    api/apidel.cpp \
    api/apihist.cpp \
    api/apithumb.cpp \
    settings.cpp \
    transparentwindow.cpp \
    information.cpp \
    authentication.cpp \
    history.cpp \
    keybindings.cpp \
    general.cpp

HEADERS  += \
    screenshot.h \
    upload.h \
    api/apirequest.h \
    api/apiauth.h \
    api/apidel.h \
    api/apihist.h \
    api/apithumb.h \
    settings.h \
    transparentwindow.h \
    information.h \
    authentication.h \
    history.h \
    keybindings.h \
    general.h

OTHER_FILES += settings

RESOURCES += \
    ../resources.qrc

INSTALLS += target

isEmpty(PREFIX) {
    PREFIX = /usr/local

    # can't do target.extra or target.commands because it will no longer
    # "auto populate" target.files with the shared libraries.
    # so this is my work-around.
    INSTALLS += check
    check.depends = target
    check.commands = $$PWD/checkShared.sh
    # also necessary
    check.path = /
}

target.path = $$PREFIX/lib
