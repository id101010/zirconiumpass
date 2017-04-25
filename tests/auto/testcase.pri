

ROOTPATH = $$PWD/../..
DESTDIR = $$ROOTPATH/build/tests

QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase c++11
CONFIG -= app_bundle

TEMPLATE = app
QMAKE_LFLAGS  += -Wl,--rpath=$$ROOTPATH/build/

LIBS += -L$$ROOTPATH/build -lzircopass
INCLUDEPATH += $$ROOTPATH/src
