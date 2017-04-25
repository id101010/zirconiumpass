#include "masterkey.h"
#include "crypto/CryptoHash.h"
#include "crypto/SymmetricCipherStream.h"
#include <QDebug>
#include <QBuffer>

Masterkey::Masterkey()
{

}

bool Masterkey::deriveKey(QString password, QByteArray transformSeed, int transformRounds, QByteArray masterSeed)
{
    //based on https://gist.github.com/msmuenchen/9318327

    QByteArray key = CryptoHash::hash(password.toUtf8(), CryptoHash::Sha256);
    QByteArray iv(16, 0);


    SymmetricCipher cipher(SymmetricCipher::Aes256, SymmetricCipher::Ecb,
                           SymmetricCipher::Encrypt);
    if (!cipher.init(transformSeed, iv)) {
        qWarning() << cipher.errorString();
        return false;
    }

    QByteArray result = key.left(16);

    if (!cipher.processInPlace(result, transformRounds)) {
        qWarning() << cipher.errorString();
        return false;
    }


    if (!cipher.init(transformSeed, iv)) {
        qWarning() << cipher.errorString();
        return false;
    }

    QByteArray result2 = key.right(16);

    if (!cipher.processInPlace(result, transformRounds)) {
        qWarning() << cipher.errorString();
        return false;
    }

    QByteArray transformKey = CryptoHash::hash(result + result2, CryptoHash::Sha256);

    CryptoHash hash(CryptoHash::Sha256);
    hash.addData(masterSeed);
    hash.addData(transformKey);
    mKey = hash.result();

    //qDebug() << "masterkey"  << mKey.toHex();

    return true;
}

bool Masterkey::encrypt(const QByteArray& iv, const QByteArray& content, QByteArray& outEncrypted) const
{
    QBuffer buf(&outEncrypted);
    if(!buf.open(QIODevice::WriteOnly)) {
        return false;
    }

    SymmetricCipherStream stream(&buf,SymmetricCipher::Aes256,SymmetricCipher::Cbc, SymmetricCipher::Encrypt);
    if(!stream.init(mKey,iv)) {
        qWarning() << stream.errorString();
        return false;
    }

    if(!stream.open(QIODevice::WriteOnly)) {
        qWarning() << stream.errorString();
        return false;
    }

    if(stream.write(content) == -1) {
        qWarning() << stream.errorString();
        outEncrypted.clear();
        return false;
    }

    stream.close();
    buf.close();
    return true;
}


bool Masterkey::decrypt(const QByteArray& iv, const QByteArray& encryptedContent, QByteArray& outDecrypted) const
{
    QBuffer buf;
    buf.setData(encryptedContent);
    if(!buf.open(QIODevice::ReadOnly)) {
        return false;
    }

    SymmetricCipherStream stream(&buf,SymmetricCipher::Aes256,SymmetricCipher::Cbc, SymmetricCipher::Decrypt);
    if(!stream.init(mKey,iv)) {
        qWarning() << stream.errorString();
        return false;
    }

    if(!stream.open(QIODevice::ReadOnly)) {
        qWarning() << stream.errorString();
        return false;
    }

    outDecrypted = stream.readAll();
    if(outDecrypted.isEmpty()) {
        qWarning() << stream.errorString();
        return false;
    }

    stream.close();
    buf.close();

    return true;
}
