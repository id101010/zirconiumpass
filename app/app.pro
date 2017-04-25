#-------------------------------------------------
#
# Project created by QtCreator 2017-03-14T11:25:47
#
#-------------------------------------------------

QT       += core gui widgets

ROOTPATH=$$PWD/..

TARGET = ZirconiumPass
TEMPLATE = app
QMAKE_LFLAGS  += -Wl,--rpath=$$ROOTPATH/build/


SOURCES += main.cpp
HEADERS +=
FORMS +=



INCLUDEPATH += $$ROOTPATH/src


DESTDIR = $$ROOTPATH/build/
LIBS+= -L$$ROOTPATH/build/ -lzircopass
