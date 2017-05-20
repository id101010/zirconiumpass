#-------------------------------------------------
#
# Project created by QtCreator 2017-03-14T11:25:47
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = zircopass
TEMPLATE = lib


SOURCES += mainwindow.cpp \
        database.cpp \
        masterkey.cpp \
        databaseopendialog.cpp \
        databasecontent.cpp \
        entriestablemodel.cpp \
        entry.cpp \
        abstractvalue.cpp \
        cryptedvalue.cpp \
        plainvalue.cpp \
        entrydialog.cpp \
        valuestablemodel.cpp

HEADERS += mainwindow.h \
        database.h \
        masterkey.h \
        databaseopendialog.h \
        databasecontent.h \
        jsonserializable.h \
        entriestablemodel.h \
        entry.h \
        abstractvalue.h \
        cryptedvalue.h \
        plainvalue.h \
        entrydialog.h \
        valuestablemodel.h

FORMS += mainwindow.ui \
         databaseopendialog.ui \
         entrydialog.ui


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

DESTDIR = $$PWD/../build/

LIBS+= -lgcrypt
