#include "databasecontent.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "entry.h"
#include "factory.h"

/**
 * @brief Constructor for this class
 * @param factory
 */
DatabaseContent::DatabaseContent(QSharedPointer<Factory> factory) : mFactory(factory)
{

}

DatabaseContent::~DatabaseContent()
{
    qDeleteAll(mEntries);
}

/**
 * @brief Encrypt the database content with the master key
 * @param masterkey
 * @return true on success
 */
bool DatabaseContent::encrypt(const Masterkey &masterkey)
{
    if(mStreamStartBytes.isEmpty() || mEncryptionIv.isEmpty()) { //TODO: check that json != empty
        return false;
    }

    // Step 1: Generate JsonDocument from the entries
    QJsonArray arr;
    for(const JsonSerializable* inst : mEntries) {
        arr.append(inst->saveToJson());
    }
    QJsonDocument doc;
    doc.setArray(arr);

    // Step 2: Prepare bytestream (prepend jsondata with some known bytes)
    QByteArray plain;
    plain.append(mStreamStartBytes);
    QByteArray json = doc.toJson();
    plain.append(json);


    // Step 3: Encrypt complete bytestream
    return masterkey.encrypt(mEncryptionIv,plain,mCrypted);
}

/**
 * @brief Decrypt the database content with the masterkey
 * @param masterkey
 * @return true on success
 */
bool DatabaseContent::decrypt(const Masterkey &masterkey)
{
    // Step 1: Decrypt database and check for correct start of stream (using known bytes)
    if(mStreamStartBytes.isEmpty() || mEncryptionIv.isEmpty() || mCrypted.isEmpty()) {
        return false;
    }

    mEntries.clear();

    QByteArray plain;
    if(!masterkey.decrypt(mEncryptionIv,mCrypted,plain)) {
        return false;
    }

    int checkBytesLength = mStreamStartBytes.length();
    if(plain.left(checkBytesLength) != mStreamStartBytes) {
        qWarning() << "StreamStartBytes mismatch. Wrong masterkey?";
        return false;
    }

    // Step 3: Parse bytestream into JsonDocument
    QByteArray json = plain.mid(checkBytesLength);

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(json,&err);
    if(err.error != QJsonParseError::NoError) {
        qWarning() << "Error while parsing json" << err.errorString();
        return false;
    }

    // Step 3: Convert Json Document into Vector of Entries

    const QJsonArray arr = doc.array();
    for(const QJsonValue val: arr) {
        if(val.isObject()) {
            const QJsonObject obj = val.toObject();
            //TODO: Check basic properties?
            Entry* instance =  mFactory->createEntry();
            Q_ASSERT(instance!=nullptr);
            if(!instance->loadFromJson(obj)) {
                qWarning() << "error while deserializing Entry. skipping." << obj;
            } else {
                mEntries.append(instance);
            }
        } else {
            qWarning() << "non object found. skipping." << val;
        }
    }

    return true;
}

/**
 * @brief Returns the encryption state of the database content
 */
bool DatabaseContent::containsDecryptedData() const
{
    return !mEntries.empty();
}

/**
 * @brief Clear all entries
 */
void DatabaseContent::clearDecryptedData()
{
    mEntries.clear();
}

/**
 * @brief Set encrypted data
 * @param encryptedData
 */
void DatabaseContent::setCrypted(const QByteArray &encryptedData)
{
    mCrypted = encryptedData;
}

/**
 * @brief Set the encryption initialization vector
 * @param iv
 */
void DatabaseContent::setEncryptionIv(const QByteArray &iv)
{
    mEncryptionIv = iv;
}

/**
 * @brief Set the streamstartbytes of the content. Those are used to check if the correct password was provided
 * @param startBytes
 */
void DatabaseContent::setStreamStartBytes(const QByteArray &startBytes)
{
    mStreamStartBytes = startBytes;
}

/**
 * @brief Return the encrypted representation of the database content
 */
QByteArray DatabaseContent::crypted() const
{
    return mCrypted;
}

/**
 * @brief Return encryption initialization vector
 */
QByteArray DatabaseContent::encryptionIv() const
{
    return mEncryptionIv;
}

/**
 * @brief Return stream start bytes
 */
QByteArray DatabaseContent::streamStartBytes() const
{
    return mStreamStartBytes;
}

/**
 * @brief Returns a vector with all database entries
 */
const QVector<Entry *> &DatabaseContent::entries() const
{
    return mEntries;
}

/**
 * @brief Add a new entry to the database
 * @param entry
 */
void DatabaseContent::addEntry(Entry *entry)
{
    Q_ASSERT(entry!=nullptr);
    mEntries.append(entry);
    emit entryAdded(mEntries.size()-1);
}

/**
 * @brief Removes an entry from the database
 * @param entry
 */
void DatabaseContent::removeEntry(Entry *entry)
{
    Q_ASSERT(entry!=nullptr);
    int ind = mEntries.indexOf(entry);
    if(ind != -1){
        mEntries.removeAt(ind);
        emit entryRemoved(ind);
        delete entry;
    }
}

/**
 * @brief Set factory
 * @param factory
 */
void DatabaseContent::setFactory(QSharedPointer<Factory> factory)
{
    mFactory.swap(factory);
}

