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

INSTALLS += target script uglobalhotkey

target.path = $$PREFIX/lib

# A little bit hacky. Would love some suggestions for something more standard...
uglobalhotkey.path = $$PREFIX/lib
unix:uglobalhotkey.files = $$OUT_PWD/../lib/uglobalhotkey/*.so*

script.path = $$PWD
unix:script.commands = $$PWD/checkShared.sh

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/uglobalhotkey/release/ -lUGlobalHotkey
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/uglobalhotkey/debug/ -lUGlobalHotkey
else:unix: LIBS += -L$$OUT_PWD/../lib/uglobalhotkey/ -lUGlobalHotkey

INCLUDEPATH += $$PWD/../lib/uglobalhotkey
DEPENDPATH += $$PWD/../lib/uglobalhotkey
