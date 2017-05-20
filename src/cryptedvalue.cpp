#include "cryptedvalue.h"

#include <QJsonArray>

CryptedValue::CryptedValue()
{

}

void CryptedValue::decrypt(const QByteArray &streamkey, std::function<void (const char *, size_t)> visitor)
{

}

void CryptedValue::setValue(const QByteArray &streamkey, const QString &value)
{

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
