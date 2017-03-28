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
Database Database::create_from_file(QString filename)
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
    in >> d.m_transform_seed; // read transform seed
    in >> d.m_master_seed; // read master seed
    in >> d.m_encryption_iv; // read ecryption initialisation vector
    in >> d.m_stream_start_bytes; // read stream start bytes (offset)
    in >> d.m_protected_stream_key; // read protected stream key
    in >> d.m_transform_rounds; // read number of transform rounds

    file.close();

    return d;
}

/**
 * Create a new, empty and initialized database object
 **/
Database Database::create_new(QString password, int rounds)
{
    Database d; // create new database

    d.m_transform_seed = QByteArray(); // read transform seed
    d.m_master_seed = QByteArray(); // read master seed
    d.m_encryption_iv = QByteArray(); // read ecryption initialisation vector
    d.m_stream_start_bytes = QByteArray(); // read stream start bytes
    d.m_protected_stream_key = QByteArray(); // read protected stream key
    d.m_transform_rounds = rounds; // read number of transform rounds

    d.m_master_key = Masterkey(); // Initialize masterkey object
    bool keycheck = d.m_master_key.derive_key(password, d.m_transform_seed, d.m_transform_rounds, d.m_master_seed); // derive key

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
    out << m_transform_seed; // Write transform seed
    out << m_master_seed; // Write master seed
    out << m_encryption_iv; // Write ecryption initialisation vector
    out << m_stream_start_bytes; // Write stream start bytes (offset)
    out << m_protected_stream_key; // Write protected stream key
    out << m_transform_rounds; // Write number of transform rounds

    file.close();
}

/**
 * Encrypt pass database
 **/
bool Database::encrypt()
{

}
