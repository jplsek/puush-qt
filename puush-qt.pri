# qt-5.7 is needed because of "qt-quick control 2".
# can't use QT_VERSION, because lessThan can't handle decimals (5.10)
MIN_MESSAGE = "This application requires a minimum verion of Qt 5.7"
lessThan(QT_MAJOR_VERSION, 5): error($${MIN_MESSAGE})
lessThan(QT_MINOR_VERSION, 7): error($${MIN_MESSAGE})

QMAKE_CXXFLAGS += -std=c++11

include($$PWD/lib/uglobalhotkey/uglobalhotkey.pri)
