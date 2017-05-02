#include "cryptedvalue.h"

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

}

bool CryptedValue::loadFromJson(const QJsonObject &obj)
{

}
