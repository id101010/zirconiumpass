#include "masterkey.h"
#include "crypto/CryptoHash.h"
#include "crypto/SymmetricCipherStream.h"
#include <QDebug>
#include <QBuffer>

Masterkey::Masterkey()
{

}

//basic master key setter/getters ahead

int Masterkey::transformRounds() const
{
    return mTransformRounds;
}

void Masterkey::setTransformRounds(int rounds)
{
    mTransformRounds = rounds;
}

void Masterkey::setTransformSeed(const QByteArray &seed)
{
    mTransformSeed = seed;
}

QByteArray Masterkey::transformSeed() const
{
    return mTransformSeed;
}

void Masterkey::setMasterSeed(const QByteArray &seed)
{
    mMasterSeed = seed;
}

QByteArray Masterkey::masterSeed() const
{
    return mMasterSeed;
}


/**
 * @brief Derives the masterkey from the passed password
 * @param password
 * @return
 */
bool Masterkey::deriveKey(QString password)
{
    //based on https://gist.github.com/msmuenchen/9318327

    QByteArray key = CryptoHash::hash(password.toUtf8(), CryptoHash::Sha256);
    QByteArray iv(16, 0);


    SymmetricCipher cipher(SymmetricCipher::Aes256, SymmetricCipher::Ecb,
                           SymmetricCipher::Encrypt);
    if (!cipher.init(mTransformSeed, iv)) {
        qWarning() << cipher.errorString();
        return false;
    }

    QByteArray result = key.left(16);

    if (!cipher.processInPlace(result, mTransformRounds)) {
        qWarning() << cipher.errorString();
        return false;
    }


    if (!cipher.init(mTransformSeed, iv)) {
        qWarning() << cipher.errorString();
        return false;
    }

    QByteArray result2 = key.right(16);

    if (!cipher.processInPlace(result, mTransformRounds)) {
        qWarning() << cipher.errorString();
        return false;
    }

    QByteArray transformKey = CryptoHash::hash(result + result2, CryptoHash::Sha256);

    CryptoHash hash(CryptoHash::Sha256);
    hash.addData(mMasterSeed);
    hash.addData(transformKey);
    mKey = hash.result();

    //qDebug() << "masterkey"  << mKey.toHex();

    return true;
}


/**
 * @brief Encrypt the passed data with the masterkey
 * @param iv initialization vector
 * @param content data to encrypt
 * @param outEncrypted where to write the encrypted data to
 * @return True on success
 */
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


/**
 * @brief Decrypt the passed data using the master key
 * @param iv initialization vector
 * @param encryptedContent encrypted data
 * @param outDecrypted where to store the decrypted data
 * @return True on success
 */
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
