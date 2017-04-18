#include "masterkey.h"
#include "crypto/CryptoHash.h"
#include "crypto/SymmetricCipher.h"
#include <QDebug>

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

    qDebug() << "masterkey"  << mKey.toHex();

    return true;
}
