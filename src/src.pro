#-------------------------------------------------
#
# Project created by QtCreator 2017-03-14T11:25:47
#
#-------------------------------------------------

QT       += core gui widgets
CONFIG += c++11
TARGET = zircopass
TEMPLATE = lib


SOURCES += mainwindow.cpp \
        database.cpp \
        masterkey.cpp \
        databasecontent.cpp \
        entriestablemodel.cpp \
        entry.cpp \
        abstractvalue.cpp \
        cryptedvalue.cpp \
        plainvalue.cpp \
        entrydialog.cpp \
        valuestablemodel.cpp \
        databasedialog.cpp \
        factory.cpp

HEADERS += mainwindow.h \
        database.h \
        masterkey.h \
        databasecontent.h \
        jsonserializable.h \
        entriestablemodel.h \
        entry.h \
        abstractvalue.h \
        cryptedvalue.h \
        plainvalue.h \
        entrydialog.h \
        valuestablemodel.h \
        databasedialog.h \
        factory.h

FORMS += mainwindow.ui \
         entrydialog.ui \
         databasedialog.ui


# Crypto stuff from KeepassXC ahead

SOURCES +=  crypto/CryptoHash.cpp \
            crypto/Random.cpp \
            crypto/SymmetricCipherGcrypt.cpp \
            crypto/Uuid.cpp \
            crypto/Crypto.cpp \
            crypto/SymmetricCipher.cpp \
            crypto/SymmetricCipherStream.cpp \
            crypto/LayeredStream.cpp


HEADERS+=   crypto/Crypto.h \
            crypto/CryptoHash.h \
            crypto/Random.h \
            crypto/SymmetricCipherBackend.h \
            crypto/SymmetricCipherGcrypt.h \
            crypto/SymmetricCipher.h \
            crypto/Uuid.h \
            crypto/SymmetricCipherStream.h \
            crypto/LayeredStream.h

DESTDIR = $$PWD/../build/

LIBS+= -lgcrypt
