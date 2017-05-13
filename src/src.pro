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
    keybindings.cpp

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
    keybindings.h

OTHER_FILES += settings

RESOURCES += \
    ../resources.qrc

INSTALLS += target

target.path = $$PREFIX/lib

isEmpty(PREFIX) {
    PREFIX = /usr/local
    target.extra = $$PWD/checkShared.sh
}
