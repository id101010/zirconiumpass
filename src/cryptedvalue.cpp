#include "cryptedvalue.h"

#include <QJsonArray>
#include <QBuffer>
#include "crypto/SymmetricCipherStream.h"
#include <QDebug>

/**
 * @brief Constructor for this class.
 */
CryptedValue::CryptedValue()
{

}

/**
 * @brief Temporarily decrypt the value and show it to the passed lambda function before overwriting the memory region again
 * @param streamkey Key used to decrypt
 * @param function Lambdafunction that should be called with the result
 */
void CryptedValue::decrypt(const QByteArray &streamkey, std::function<void (const QString&)> visitor)
{
    QByteArray outDecrypted;

    if(!mValue.isEmpty()) { // crypted content
        QBuffer buf;
        buf.setData(mValue);
        if(!buf.open(QIODevice::ReadOnly)) {
            return;
        }

        QByteArray iv ="E830094B97205D2A"; //constant iv
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

/**
 * @brief Sets a new value
 * @param streamkey Key used to encrypt
 * @param value new Value to set
 */
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

/**
 * @brief Serializes the instance to a JSON Object
 */
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

/**
 * @brief Restore the cryptedvalue from a JSON object
 */
bool CryptedValue::loadFromJson(const QJsonObject &obj)
{
    if(!AbstractValue::loadFromJson(obj)) {
        return false;
    }


    QJsonArray cryptArr;
    QByteArray cryptBytes;

    /* try to load encrypted bytearray from json */
    if(obj["cryptedValue"].isArray()){
        cryptArr = obj["cryptedValue"].toArray();
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

/**
 * @brief Returns the type of this object
 */
AbstractValue::Type CryptedValue::type() const
{
    return AbstractValue::Type::encrypted;
}

/**
 * @brief Returns true if the crypted value is empty
 */
bool CryptedValue::isEmpty() const
{
    return mValue.isEmpty();
}

/**
 * @brief Returns a display representation of the value. The display representation will never show the actual password
 */
QString CryptedValue::displayValue() const
{
    if(isEmpty()) {
        return "<emtpy>";
    } else {
        return "*********";
    }
}
