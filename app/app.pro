include(../defaults.pri)

lessThan(QT_MAJOR_VERSION, 5): error("This application requires a minimum verion of Qt 5.0")

QT += core gui network widgets

TEMPLATE = app
TARGET = puush-qt

SOURCES += main.cpp

LIBS += -L../src -lpuush-qt

isEmpty(PREFIX) {
    PREFIX = /usr/local
}

INSTALLS += target desktop icon

target.path = $$PREFIX/bin

desktop.path = /usr/share/applications
desktop.files += $$SRC_DIR/$$TARGET.desktop

icon.path = /usr/share/icons/hicolor/scalable/apps
icon.files += $$SRC_DIR/images/$$TARGET.svg
# mostly optional, update icon cache
unix:icon.extra += gtk-update-icon-cache /usr/share/icons/hicolor
