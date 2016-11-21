TEMPLATE = app

include(../defaults.pri)

QT += widgets testlib network

SOURCES += main.cpp

LIBS += -L../src -lpuush-qt
