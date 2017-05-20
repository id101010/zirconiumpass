#include "databasecontent.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "entry.h"

DatabaseContent::DatabaseContent(QSharedPointer<DatabaseFactory> factory) : mFactory(factory)
{

}

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

bool DatabaseContent::containsDecryptedData() const
{
    return !mEntries.empty();
}

void DatabaseContent::clearDecryptedData()
{
    mEntries.clear();
}

void DatabaseContent::setCrypted(const QByteArray &encryptedData)
{
    mCrypted = encryptedData;
}

void DatabaseContent::setEncryptionIv(const QByteArray &iv)
{
    mEncryptionIv = iv;
}

void DatabaseContent::setStreamStartBytes(const QByteArray &startBytes)
{
    mStreamStartBytes = startBytes;
}

QByteArray DatabaseContent::crypted() const
{
    return mCrypted;
}

QByteArray DatabaseContent::encryptionIv() const
{
    return mEncryptionIv;
}

QByteArray DatabaseContent::streamStartBytes() const
{
    return mStreamStartBytes;
}

const QVector<Entry *> &DatabaseContent::entries() const
{
    return mEntries;
}

void DatabaseContent::addEntry(Entry *entry)
{
    Q_ASSERT(entry!=nullptr);
    mEntries.append(entry);
    emit entryAdded(mEntries.size()-1);
}

void DatabaseContent::removeEntry(Entry *entry)
{
    Q_ASSERT(entry!=nullptr);
    int ind = mEntries.indexOf(entry);
    if(ind != -1){
        mEntries.removeAt(ind);
        emit entryRemoved(ind);
    }
}

void DatabaseContent::setFactory(QSharedPointer<DatabaseFactory> factory)
{
    mFactory.swap(factory);
}

DatabaseFactory::~DatabaseFactory()
{

}

Entry *DatabaseFactory::createEntry()
{
    return new Entry();
}
