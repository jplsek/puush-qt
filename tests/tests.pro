lessThan(QT_MAJOR_VERSION, 5): error("This application requires a minimum verion of Qt 5.0")

QT += widgets testlib network

TEMPLATE = app

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/release/ -lpuush-qt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/debug/ -lpuush-qt
else:unix: LIBS += -L$$OUT_PWD/../src/ -lpuush-qt

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/uglobalhotkey/release/ -lUGlobalHotkey
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/uglobalhotkey/debug/ -lUGlobalHotkey
else:unix: LIBS += -L$$OUT_PWD/../lib/uglobalhotkey/ -lUGlobalHotkey

INCLUDEPATH += $$PWD/../lib/uglobalhotkey
DEPENDPATH += $$PWD/../lib/uglobalhotkey

INCLUDEPATH += $$PWD/../src
DEPENDPATH += $$PWD/../src
