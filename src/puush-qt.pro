lessThan(QT_MAJOR_VERSION, 5): error("This application requires a minimum verion of Qt 5.0")

QT += core gui network widgets

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

INSTALLS += target desktop icon

target.path = $$PREFIX/bin

desktop.path = /usr/share/applications
desktop.files += $${TARGET}.desktop

icon.path = /usr/share/icons/hicolor/scalable/apps
icon.files += images/$${TARGET}.svg
# mostly optional, update icon cache
unix:icon.extra += gtk-update-icon-cache /usr/share/icons/hicolor
