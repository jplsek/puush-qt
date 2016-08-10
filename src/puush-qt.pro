QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = puush-qt
TEMPLATE = app

SOURCES += main.cpp \
    window.cpp \
    screenshot.cpp \
    upload.cpp

HEADERS  += \
    window.h \
    screenshot.h \
    upload.h

RESOURCES += \
    systray.qrc

isEmpty(PREFIX) {
    PREFIX = /usr/local
}

target.path = $$PREFIX/bin

INSTALLS += target
