lessThan(QT_MAJOR_VERSION, 5): error("This application requires a minimum verion of Qt 5.0")

QT += core gui network widgets

TEMPLATE = app
TARGET = puush-qt

SOURCES += main.cpp

isEmpty(PREFIX) {
    PREFIX = /usr/local
}

INSTALLS += target desktop icon

target.path = $$PREFIX/bin

desktop.path = /usr/share/applications
desktop.files += $$PWD/../$$TARGET.desktop

icon.path = /usr/share/icons/hicolor/scalable/apps
icon.files += $$PWD/../images/$$TARGET.svg
# mostly optional, update icon cache
unix:icon.extra += gtk-update-icon-cache /usr/share/icons/hicolor

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/release/ -lpuush-qt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/debug/ -lpuush-qt
else:unix: LIBS += -L$$OUT_PWD/../src/ -lpuush-qt

INCLUDEPATH += $$PWD/../src
DEPENDPATH += $$PWD/../src
