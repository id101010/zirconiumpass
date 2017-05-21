#include "abstractvalue.h"
#include <QMetaEnum>
#include <QVariant>

AbstractValue::AbstractValue()
{

}

const QString &AbstractValue::name() const
{
    return mName;
}

void AbstractValue::setName(const QString &name)
{
    mName = name;
}

QJsonObject AbstractValue::saveToJson() const
{
    QJsonObject val;
    val["type"] = QVariant::fromValue<AbstractValue::Type>(this->type()).toString(); //Convert enum to string
    val["name"] = this->name();

    return val;
}

bool AbstractValue::loadFromJson(const QJsonObject &obj)
{
    /* try to load name */
    if(obj["name"].isString()){
        setName(obj["name"].toString());
    } else {
        return false;
    }

    /* validate that type equals to the type provided by the current instance subclass */

    QMetaEnum me = QMetaEnum::fromType<Type>();
    bool castOk = false;

    //Convert type string into enum using QMetaEnum reflection magic
    Type typeInJson = static_cast<Type>(me.keyToValue(obj["type"].toString().toStdString().c_str(),&castOk));
    if(!castOk) {
        return false;
    }

    if(typeInJson != type()) {
        return false;
    }

    return true;
}

