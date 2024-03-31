# awu.pro
TEMPLATE = app
TARGET = awu
INCLUDEPATH += /usr/include/qt
CONFIG += c++11
QT += widgets

HEADERS += addGame.h
SOURCES += *.cpp

target.path = /usr/bin
INSTALLS += target
