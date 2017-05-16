# qt-5.7 is needed because of "qt-quick control 2".
lessThan(QT_VERSION, 5.7): error("This application requires a minimum verion of Qt 5.7")

QMAKE_CXXFLAGS += -std=c++11

include($$PWD/lib/uglobalhotkey/uglobalhotkey.pri) 
