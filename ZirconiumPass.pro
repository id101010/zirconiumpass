#-------------------------------------------------
#
# Project created by QtCreator 2017-03-14T11:25:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ZirconiumPass
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
        database.cpp \
        masterkey.cpp \
        databaseopendialog.cpp \
        databasecontent.cpp

HEADERS += mainwindow.h \
        database.h \
        masterkey.h \
        databaseopendialog.h \
        databasecontent.h \
        jsonserializable.h

FORMS += mainwindow.ui \
         databaseopendialog.ui


# Crypto stuff from KeepassXC ahead

SOURCES +=  crypto/CryptoHash.cpp \
            crypto/Random.cpp \
            crypto/SymmetricCipherGcrypt.cpp \
            crypto/Uuid.cpp \
            crypto/Crypto.cpp \
            crypto/SymmetricCipher.cpp \
            crypto/SymmetricCipherStream.cpp \
            crypto/LayeredStream.cpp


            #crypto/SymmetricCipherSalsa20.cpp \


HEADERS+=   crypto/Crypto.h \
            crypto/CryptoHash.h \
            crypto/Random.h \
            crypto/SymmetricCipherBackend.h \
            crypto/SymmetricCipherGcrypt.h \
            crypto/SymmetricCipher.h \
            crypto/Uuid.h \
            crypto/SymmetricCipherStream.h \
            crypto/LayeredStream.h
#            crypto/SymmetricCipherSalsa20.h \

LIBS+= -lgcrypt


