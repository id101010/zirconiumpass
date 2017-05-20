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
    QJsonObject val = AbstractValue::saveToJson();
    val["plainValue"] = mValue;
    return val;
}

bool PlainValue::loadFromJson(const QJsonObject &obj)
{
    /* try to load plainValue */
    if(obj["plainValue"].isString()){
        mValue = obj["plainValue"].toString();
    } else {
        return false;
    }

    /* try to load name */
    if(obj["name"].isString()){
        this->setName(obj["name"].toString());
    } else {
        return false;
    }
}

const QString& PlainValue::type() const
{
    static QString store = "plain";
    return store;
}
