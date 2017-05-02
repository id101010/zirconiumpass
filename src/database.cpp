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

    //Temporaries for database content
    QByteArray crypted;
    QByteArray encryptionIv;
    QByteArray streamStartBytes;

    //Temporaries for masterkey
    QByteArray transformSeed;
    int transformRounds;
    QByteArray masterSeed;


    in >> transformSeed; // read transform seed
    in >> masterSeed; // read master seed
    in >> encryptionIv; // read ecryption initialisation vector
    in >> streamStartBytes; // read stream start bytes (check)
    in >> d.mProtectedStreamKey; // read protected stream key
    in >> transformRounds; // read number of transform rounds
    in >> crypted;

    file.close();

    d.mMasterKey.setTransformSeed(transformSeed);
    d.mMasterKey.setTransformRounds(transformRounds);
    d.mMasterKey.setMasterSeed(masterSeed);


    d.mDatabaseContent.setEncryptionIv(encryptionIv);
    d.mDatabaseContent.setStreamStartBytes(streamStartBytes);
    d.mDatabaseContent.setCrypted(crypted);

    return d;
}

/**
 * Create a new, empty and initialized database object
 **/
Database Database::createNew(QString password, int rounds)
{
    Database d; // create new database


    d.mProtectedStreamKey = randomGen()->randomArray(32);


    d.mMasterKey = Masterkey();
    d.mMasterKey.setTransformSeed(randomGen()->randomArray(32));
    d.mMasterKey.setTransformRounds(rounds);
    d.mMasterKey.setMasterSeed(randomGen()->randomArray(32));
    bool keycheck = d.mMasterKey.deriveKey(password); // derive key

    // Check if master key was derived properly
    if(!keycheck){
        qDebug() << "Master key derivation failed!";
        return Database();
    }

    d.mDatabaseContent.setEncryptionIv(randomGen()->randomArray(16));
    d.mDatabaseContent.setStreamStartBytes(randomGen()->randomArray(32));

    return d;
}

/**
 * Decrypt pass database object
 **/
bool Database::decrypt(QString password)
{

    bool keycheck = mMasterKey.deriveKey(password); // derive key

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
    out << mMasterKey.transformSeed(); // Write transform seed
    out << mMasterKey.masterSeed(); // Write master seed
    out << mDatabaseContent.encryptionIv(); // Write ecryption initialisation vector
    out << mDatabaseContent.streamStartBytes(); // Write stream start bytes (offset)
    out << mProtectedStreamKey; // Write protected stream key
    out << mMasterKey.transformRounds(); // Write number of transform rounds

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
