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
 * @brief Decrypt the crypted member of this class by providing a lambda function which handles the decryption.
 *        The memorysector is protected and gets filled with zeros after the decryption has happened.
 * @param streamkey Key used to decrypt
 * @param function Lambdafunction for decryption
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

/**
 * @brief Takes a value and encrypts it
 * @param streamkey Key used to encrypt
 * @param value Value to encrypt
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
 * @brief Returns a JSON Object of this cryptedvalue object
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
 * @brief Load a cryptedvalue object from a JSON object
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
 * @brief Displays eigther stars or <empty> depending on the member mValue
 */
QString CryptedValue::displayValue() const
{
    if(isEmpty()) {
        return "<emtpy>";
    } else {
        return "*********";
    }
}
