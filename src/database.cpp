#include "database.h"

#include <QByteArray>
#include <QDataStream>
#include <QFile>
#include <QDebug>
#include "crypto/Random.h"

static const int VERSION = 1;

/**
 * @brief Creates a unintialized database. This method is Private and for internal use only
 */
Database::Database() : mFactory(QSharedPointer<Factory>::create()), mDatabaseContent(mFactory)
{

}

/**
 * @brief Create a database instance from a existing database file.
 * @param filename absolut file name
 * @return A unique_ptr to the loaded database
 **/
std::unique_ptr<Database> Database::createFromFile(const QString &filename)
{
    char magic[14]; // magic number
    qint32 version; // version

    QFile file(filename); // Create a file object from filename

    // Open file as read only and check if file is opened
    if(!file.open(QIODevice::ReadOnly)){
        qWarning() << "Could not open file!";
        return {}; // return empty db
    }

    QDataStream in(&file); // Link datastream to in

    // Try to read magic number
    if(in.readRawData(magic, 14) != 14 || strncmp(magic, "zirconiumpass", 14) != 0){
        qWarning() << "Magic number invalid!";
        return {}; // return empty db
    }

    // Read and check version
    in >> version;
    if(version != VERSION){
        qWarning() << "Version does not match!";
        return {}; // return empty db
    }

    // Create new database and read header data from file
    std::unique_ptr<Database> d(new Database());

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
    in >> d->mProtectedStreamKey; // read protected stream key
    in >> transformRounds; // read number of transform rounds
    in >> crypted;

    file.close();

    d->mMasterKey.setTransformSeed(transformSeed);
    d->mMasterKey.setTransformRounds(transformRounds);
    d->mMasterKey.setMasterSeed(masterSeed);


    d->mDatabaseContent.setEncryptionIv(encryptionIv);
    d->mDatabaseContent.setStreamStartBytes(streamStartBytes);
    d->mDatabaseContent.setCrypted(crypted);

    return d;
}

/**
 * @brief Create a new, empty and initialized database object
 * @param password Password for the database encryption
 * @param rounds Number of Key-Transformation rounds
 **/
std::unique_ptr<Database> Database::createNew(const QString &password, int rounds)
{
    std::unique_ptr<Database> d(new Database()); // create new database

    d->mProtectedStreamKey = randomGen()->randomArray(32);

    d->mMasterKey = Masterkey();
    d->mMasterKey.setTransformSeed(randomGen()->randomArray(32));
    d->mMasterKey.setTransformRounds(rounds);
    d->mMasterKey.setMasterSeed(randomGen()->randomArray(32));
    bool keycheck = d->mMasterKey.deriveKey(password); // derive key

    // Check if master key was derived properly
    if(!keycheck){
        qDebug() << "Master key derivation failed!";
        return {};
    }

    d->mDatabaseContent.setEncryptionIv(randomGen()->randomArray(16));
    d->mDatabaseContent.setStreamStartBytes(randomGen()->randomArray(32));

    return d;
}

/**
 * @brief Decrypt the database content
 * @param password masterpassword
 * @return true on success
 **/
bool Database::decrypt(const QString& password)
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
 * @brief Write encrypted pass database object to file
 * @param filename absolute file name
 * @return true on success
 **/
bool Database::write(const QString& filename)
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

/**
 * @brief Returns the database content
 */
DatabaseContent& Database::databaseContent()
{
    return mDatabaseContent;
}

/**
 * @brief Returns the database content as a constant value
 */
const DatabaseContent& Database::databaseContent() const
{
    return mDatabaseContent;
}

/**
 * @brief Returns the protected stream key, used to decrypt the actual passwords from the JSON
 */
const QByteArray &Database::protectedStreamKey() const
{
    return mProtectedStreamKey;
}

/**
 * @brief Set Factory (used for testing)
 * @param factory
 */
void Database::setFactory(QSharedPointer<Factory> factory)
{
    mDatabaseContent.setFactory(factory);
    mFactory = factory;
}

/**
 * @brief Returns a pointer to the used factory
 */
QSharedPointer<Factory> Database::factory() const
{
    return mFactory;
}

/**
 * @brief Encrypt pass database
 **/
bool Database::encrypt()
{
    return mDatabaseContent.encrypt(mMasterKey);
}
