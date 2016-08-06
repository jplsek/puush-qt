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

FORMS    += \
    puushqt.ui

RESOURCES += \
    systray.qrc

target.path = /usr/local/bin/
INSTALLS += target
