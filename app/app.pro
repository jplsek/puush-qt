include($$PWD/../puush-qt.pri)

QT += core network widgets qml quick

TEMPLATE = app
TARGET = puush-qt

SOURCES += main.cpp \
    systray.cpp

HEADERS += systray.h

isEmpty(PREFIX) {
    PREFIX = /usr/local
}

RESOURCES += \
    ../resources.qrc

RC_FILE = puush-qt.rc

INSTALLS += target desktop icon

target.path = $$PREFIX/bin

desktop.path = /usr/share/applications
desktop.files = $$PWD/puush-qt.desktop

icon.path = /usr/share/icons/hicolor/scalable/apps
icon.files += $$PWD/../resources/images/puush-qt.svg
# mostly optional, update icon cache
unix:icon.extra += $$PWD/updateIconCache.sh

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/release/ -lpuush-qt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/debug/ -lpuush-qt
else:unix: LIBS += -L$$OUT_PWD/../src/ -lpuush-qt

INCLUDEPATH += $$PWD/../src
DEPENDPATH += $$PWD/../src
