#ifndef DATABASECONTENT_H
#define DATABASECONTENT_H

#include "masterkey.h"
#include <QVector>
#include <QSharedPointer>
#include <jsonserializable.h>


struct DatabaseFactory {
    virtual ~DatabaseFactory();
    virtual JsonSerializable* createEntry();
};

class DatabaseContent
{
public:
    DatabaseContent(QSharedPointer<DatabaseFactory> factory);
    bool encrypt(const Masterkey& masterkey);
    bool decrypt(const Masterkey& masterkey);
    bool containsDecryptedData() const;
    void clearDecryptedData();

    void setCrypted(const QByteArray& encryptedData);
    void setEncryptionIv(const QByteArray& iv);
    void setStreamStartBytes(const QByteArray& startBytes);

    QByteArray crypted() const;
    QByteArray encryptionIv() const;
    QByteArray streamStartBytes() const;

    const QVector<JsonSerializable*>& entires() const;
    void addEntry(JsonSerializable* entry);
    void removeEntry(JsonSerializable* entry);



    void setFactory(QSharedPointer<DatabaseFactory> factory); //for testing purposes

 private:
    QSharedPointer<DatabaseFactory> mFactory;
    QVector<JsonSerializable*> mEntries;
    QByteArray mCrypted;
    QByteArray mEncryptionIv;
    QByteArray mStreamStartBytes;
};

#endif // DATABASECONTENT_H
