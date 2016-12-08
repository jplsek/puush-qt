include(../defaults.pri)

lessThan(QT_MAJOR_VERSION, 5): error("This application requires a minimum verion of Qt 5.0")

QT += core gui network widgets

TEMPLATE = lib
TARGET = puush-qt

SOURCES += window.cpp \
    screenshot.cpp \
    upload.cpp \
    api/apirequest.cpp \
    api/apiauth.cpp \
    api/apidel.cpp \
    api/apihist.cpp \
    api/apithumb.cpp \
    settings.cpp \
    transparentwindow.cpp

HEADERS  += \
    window.h \
    screenshot.h \
    upload.h \
    api/apirequest.h \
    api/apiauth.h \
    api/apidel.h \
    api/apihist.h \
    api/apithumb.h \
    settings.h \
    transparentwindow.h

RESOURCES += \
    systray.qrc

OTHER_FILES += settings

isEmpty(PREFIX) {
    PREFIX = /usr/local
}

INSTALLS += target script

target.path = $$PREFIX/lib

script.path = $$PWD
script.commands = $$PWD/checkShared.sh
