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
    QJsonObject val;
    val["type"] = this->type();
    val["name"] = this->name();

    QJsonArray arr;
    for(int i=0; i<mValue.size(); i++){
        arr.append(mValue.at(i));
    }

    val["cryptedValue"] = arr;
    return val;
}

bool CryptedValue::loadFromJson(const QJsonObject &obj)
{

}

const QString &CryptedValue::type() const
{
    static QString store = "encrypted";
    return store;
}
