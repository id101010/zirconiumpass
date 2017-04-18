#include "database.h"

#include <QByteArray>
#include <QDataStream>
#include <QFile>
#include <QDebug>

static const int VERSION = 1;

Database::Database()
{

}

/**
 * Decrypt and parse a pass database stored as file
 **/
Database Database::createFromFile(QString filename)
{
    char magic[14]; // magic number
    qint32 version; // version

    QFile file(filename); // Create a file object from filename

    // Open file as read only and check if file is opened
    if(!file.open(QIODevice::ReadOnly)){
        qWarning() << "Could not open file!";
        return Database(); // return empty db
    }

    QDataStream in(&file); // Link datastream to in

    // Try to read magic number
    if(in.readRawData(magic, 14) != 14 || strncmp(magic, "zirconiumpass", 14) != 0){
        qWarning() << "Magic number invalid!";
        return Database(); // return empty db
    }

    // Read and check version
    in >> version;
    if(version != VERSION){
        qWarning() << "Version does not match!";
        return Database(); // return empty db
    }

    // Create new database and read header data from file
    Database d;
    in >> d.mTransformSeed; // read transform seed
    in >> d.mMasterSeed; // read master seed
    in >> d.mEncryptionIv; // read ecryption initialisation vector
    in >> d.mStreamStartBytes; // read stream start bytes (check)
    in >> d.mProtectedStreamKey; // read protected stream key
    in >> d.mTransformRounds; // read number of transform rounds

    file.close();

    return d;
}

/**
 * Create a new, empty and initialized database object
 **/
Database Database::createNew(QString password, int rounds)
{
    Database d; // create new database

    d.mTransformSeed = QByteArray(); // read transform seed
    d.mMasterSeed = QByteArray(); // read master seed
    d.mEncryptionIv = QByteArray(); // read ecryption initialisation vector
    d.mStreamStartBytes = QByteArray(); // read stream start bytes
    d.mProtectedStreamKey = QByteArray(); // read protected stream key
    d.mTransformRounds = rounds; // read number of transform rounds

    d.mMasterKey = Masterkey(); // Initialize masterkey object
    bool keycheck = d.mMasterKey.deriveKey(password, d.mTransformSeed, d.mTransformRounds, d.mMasterSeed); // derive key

    // Check if master key was derived properly
    if(!keycheck){
        qDebug() << "Master key derivation failed!";
        return Database();
    }

    return d;
}

/**
 * Decrypt pass database object
 **/
bool Database::decrypt(QString password)
{

}

/**
 * Write encrypted pass database object to file
 **/
bool Database::write(QString filename)
{
    QFile file(filename); // Create a file object from filename

    // Open file as truncate and write only and check if file is opened
    if(!file.open(QIODevice::Truncate | QIODevice::WriteOnly)){
        qWarning() << "Could not open file!";
        return false; // return error
    }

    QDataStream out(&file); // Link datastream to out

    // Write a header with a "magic number" and a version
    out.writeRawData("zirconiumpass", 14); // Magic bytes: 697a 6b72 6e6f 7569 706d 7361 0073
    out << (qint32)VERSION; // Write version
    out << mTransformSeed; // Write transform seed
    out << mMasterSeed; // Write master seed
    out << mEncryptionIv; // Write ecryption initialisation vector
    out << mStreamStartBytes; // Write stream start bytes (offset)
    out << mProtectedStreamKey; // Write protected stream key
    out << mTransformRounds; // Write number of transform rounds

    file.close();
}

/**
 * Encrypt pass database
 **/
bool Database::encrypt()
{

}
