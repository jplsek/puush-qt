QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = puush-qt
TEMPLATE = app

SOURCES += main.cpp \
    window.cpp \
    screenshot.cpp \
    upload.cpp \
    api/apirequest.cpp \
    api/apiauth.cpp \
    api/apidel.cpp \
    api/apihist.cpp \
    api/apithumb.cpp

HEADERS  += \
    window.h \
    screenshot.h \
    upload.h \
    api/apirequest.h \
    api/apiauth.h \
    api/apidel.h \
    api/apihist.h \
    api/apithumb.h

RESOURCES += \
    systray.qrc

isEmpty(PREFIX) {
    PREFIX = /usr/local
}

target.path = $$PREFIX/bin

INSTALLS += target
