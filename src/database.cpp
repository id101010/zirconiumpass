#include "database.h"

#include <QByteArray>
#include <QDataStream>
#include <QFile>
#include <QDebug>
#include "crypto/Random.h"

static const int VERSION = 1;

Database::Database() : mDatabaseContent(QSharedPointer<DatabaseFactory>::create())
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

    QByteArray crypted;
    in >> crypted;

    file.close();


    d.mDatabaseContent.setEncryptionIv(d.mEncryptionIv);
    d.mDatabaseContent.setStreamStartBytes(d.mStreamStartBytes);
    d.mDatabaseContent.setCrypted(crypted);

    return d;
}

/**
 * Create a new, empty and initialized database object
 **/
Database Database::createNew(QString password, int rounds)
{
    Database d; // create new database

    //Props for generating the masterkey out of the password
    d.mTransformSeed =  randomGen()->randomArray(32);
    d.mTransformRounds = rounds; // read number of transform rounds
    d.mMasterSeed =  randomGen()->randomArray(32);



    d.mEncryptionIv = randomGen()->randomArray(16);
    d.mStreamStartBytes = randomGen()->randomArray(32);
    d.mProtectedStreamKey = randomGen()->randomArray(32);


    d.mMasterKey = Masterkey(); // Initialize masterkey object
    bool keycheck = d.mMasterKey.deriveKey(password, d.mTransformSeed, d.mTransformRounds, d.mMasterSeed); // derive key

    // Check if master key was derived properly
    if(!keycheck){
        qDebug() << "Master key derivation failed!";
        return Database();
    }

    d.mDatabaseContent.setEncryptionIv(d.mEncryptionIv);
    d.mDatabaseContent.setStreamStartBytes(d.mStreamStartBytes);

    return d;
}

/**
 * Decrypt pass database object
 **/
bool Database::decrypt(QString password)
{

    mMasterKey = Masterkey(); // Initialize masterkey object
    bool keycheck = mMasterKey.deriveKey(password, mTransformSeed, mTransformRounds, mMasterSeed); // derive key

    // Check if master key was derived properly
    if(!keycheck){
        qDebug() << "Master key derivation failed!";
        return false;
    }

    return mDatabaseContent.decrypt(mMasterKey);

}

/**
 * Write encrypted pass database object to file
 **/
bool Database::write(QString filename)
{

    if(!encrypt()) {
        return false;
    }

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

    out << mDatabaseContent.crypted();

    file.close();

    return true;
}

DatabaseContent &Database::databaseContent()
{
    return mDatabaseContent;
}

const DatabaseContent &Database::databaseContent() const
{
    return mDatabaseContent;
}

void Database::setFactory(QSharedPointer<DatabaseFactory> factory)
{
    mDatabaseContent.setFactory(factory);
}

/**
 * Encrypt pass database
 **/
bool Database::encrypt()
{
    return mDatabaseContent.encrypt(mMasterKey);
}
