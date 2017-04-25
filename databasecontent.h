#ifndef DATABASECONTENT_H
#define DATABASECONTENT_H

#include "masterkey.h"
#include <QVector>

class Entry;

class DatabaseContent
{
public:
    DatabaseContent();
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

    const QVector<Entry*>& entires() const;
    void addEntry(Entry* entry);
    void removeEntry(Entry* entry);


 private:
    QVector<Entry*> mEntries;
    QByteArray mCrypted;
    QByteArray mEncryptionIv;
    QByteArray mStreamStartBytes;
};

#endif // DATABASECONTENT_H
