#include "cryptedvalue.h"

#include <QJsonArray>
#include <QBuffer>
#include "crypto/SymmetricCipherStream.h"
#include <QDebug>

CryptedValue::CryptedValue()
{

}

void CryptedValue::decrypt(const QByteArray &streamkey, std::function<void (const QString&)> visitor)
{
    QByteArray outDecrypted;

    if(!mValue.isEmpty()) { // crypted content
        QBuffer buf;
        buf.setData(mValue);
        if(!buf.open(QIODevice::ReadOnly)) {
            return;
        }

        QByteArray iv ="E830094B97205D2A";
        iv = QByteArray::fromHex(iv);

        SymmetricCipherStream stream(&buf,SymmetricCipher::Salsa20,SymmetricCipher::Stream, SymmetricCipher::Decrypt);
        if(!stream.init(streamkey,iv)) {
            qWarning() << stream.errorString();
            return;
        }

        if(!stream.open(QIODevice::ReadOnly)) {
            qWarning() << stream.errorString();
            return;
        }

        outDecrypted= stream.readAll();
        if(outDecrypted.isEmpty()) {
            qWarning() << stream.errorString();
            return;
        }

        stream.close();
        buf.close();
        }


    QString decryptedString  = QString::fromUtf8(outDecrypted);
    visitor(decryptedString); //let visitor look at it

    decryptedString.fill('0'); //overwrite memory
    outDecrypted.fill('0');
}

void CryptedValue::setValue(const QByteArray &streamkey, const QString &value)
{
    if(value.isEmpty()) {
        mValue.clear();
        return;
    }
    QByteArray newCryptedValue;
    QBuffer buf(&newCryptedValue);
    if(!buf.open(QIODevice::WriteOnly)) {
        return;
    }

    QByteArray iv ="E830094B97205D2A";
    iv = QByteArray::fromHex(iv);

    SymmetricCipherStream stream(&buf,SymmetricCipher::Salsa20,SymmetricCipher::Stream, SymmetricCipher::Encrypt);
    if(!stream.init(streamkey,iv)) {
        qWarning() << stream.errorString();
        return;
    }

    if(!stream.open(QIODevice::WriteOnly)) {
        qWarning() << stream.errorString();
        return;
    }

    if(stream.write(value.toUtf8()) == -1) {
        qWarning() << stream.errorString();
        return;
    }

    stream.close();
    buf.close();
    mValue = newCryptedValue;
}

QJsonObject CryptedValue::saveToJson() const
{
    QJsonObject val = AbstractValue::saveToJson();
    QJsonArray arr;

    for(int i=0; i<mValue.size(); i++){
        arr.append(mValue.at(i));
    }

    val["cryptedValue"] = arr;
    return val;
}

bool CryptedValue::loadFromJson(const QJsonObject &obj)
{
    QJsonArray cryptArr;
    QByteArray cryptBytes;

    /* try to load encrypted bytearray from json */
    if(obj["cryptedValue"].isArray()){
        cryptArr = obj["cryptedValue"].toArray();
    } else {
        return false;
    }

    /* try to load name */
    if(obj["name"].isString()){
        this->setName(obj["name"].toString());
    } else {
        return false;
    }

    /* load encrypted bytes */
    for(int i=0; i<cryptArr.size(); i++){
        if(cryptArr[i].isDouble()){
            cryptBytes.append(cryptArr[i].toInt());
        } else {
            return false;
        }
    }

    /* save encrypted byte array */
    mValue = cryptBytes;

    return true;
}

const QString &CryptedValue::type() const
{
    static QString store = "encrypted";
    return store;
}

bool CryptedValue::isEmpty() const
{
    return mValue.isEmpty();
}


QString CryptedValue::displayValue() const
{
    if(isEmpty()) {
        return "<emtpy>";
    } else {
        return "*********";
    }
}
