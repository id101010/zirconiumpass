#include "plainvalue.h"

PlainValue::PlainValue()
{

}

const QString &PlainValue::value()
{

}

void PlainValue::setValue(const QString &value)
{
    this->mValue = value;
}

QJsonObject PlainValue::saveToJson() const
{
    QJsonObject val;
    val["type"] = this->type();
    val["name"] = this->name();
    val["plainValue"] = mValue;
    return val;
}

bool PlainValue::loadFromJson(const QJsonObject &obj)
{

}

const QString& PlainValue::type() const
{
    static QString store = "plain";
    return store;
}
