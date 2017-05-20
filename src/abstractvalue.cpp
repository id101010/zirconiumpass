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

}

bool AbstractValue::loadFromJson(const QJsonObject &obj)
{

}
