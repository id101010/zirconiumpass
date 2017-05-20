#include "abstractvalue.h"

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
    val["type"] = this->type();
    val["name"] = this->name();

    return val;
}

bool AbstractValue::loadFromJson(const QJsonObject &obj)
{
    //TODO Aaron: Move common stuff from subclasses here
}

