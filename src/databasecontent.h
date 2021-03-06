#ifndef DATABASECONTENT_H
#define DATABASECONTENT_H

#include "masterkey.h"
#include <QVector>
#include <QSharedPointer>
#include "entry.h"

/**
 * @brief The DatabaseContent class contains the encrypted/decrypted JSON representation of the database
 */
class DatabaseContent : public QObject
{
    Q_OBJECT
public:
    DatabaseContent(QSharedPointer<Factory> factory);
    ~DatabaseContent();

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

    const QVector<class Entry *> &entries() const;
    void addEntry(Entry *entry);
    void removeEntry(Entry* entry);


    void setFactory(QSharedPointer<Factory> factory); //for testing purposes

 signals:
    void entryAdded(int index);
    void entryRemoved(int index);

 private:
    QSharedPointer<Factory> mFactory;
    QVector<Entry*> mEntries;
    QByteArray mCrypted;
    QByteArray mEncryptionIv;
    QByteArray mStreamStartBytes;
};

#endif // DATABASECONTENT_H
