#include "databasecontent.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

DatabaseContent::DatabaseContent()
{

}

bool DatabaseContent::encrypt(const Masterkey &masterkey)
{
    if(mStreamStartBytes.isEmpty() || mEncryptionIv.isEmpty()) { //TODO: check that json != empty
        return false;
    }

    QByteArray plain;
    plain.append(mStreamStartBytes);

    //TODO: entries -> doc -> json

    QByteArray json = "{\"a\":1, \"b\":2}";
    plain.append(json);


    return masterkey.encrypt(mEncryptionIv,plain,mCrypted);
}

bool DatabaseContent::decrypt(const Masterkey &masterkey)
{
    if(mStreamStartBytes.isEmpty() || mEncryptionIv.isEmpty() || mCrypted.isEmpty()) {
        return false;
    }

    QByteArray plain;
    if(!masterkey.decrypt(mEncryptionIv,mCrypted,plain)) {
        return false;
    }

    int checkBytesLength = mStreamStartBytes.length();
    if(plain.left(checkBytesLength) != mStreamStartBytes) {
        qWarning() << "StreamStartBytes mismatch. Wrong masterkey?";
        return false;
    }

    QByteArray json = plain.mid(checkBytesLength);

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(json,&err);
    if(err.error != QJsonParseError::NoError) {
        qWarning() << "Error while parsing json" << err.errorString();
        return false;
    }

    qDebug() << doc.object();

    //TODO: doc -> entries

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

const QVector<Entry *> &DatabaseContent::entires() const
{
    return mEntries;
}
