#include "abstractvalue.h"
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
    val["type"] = QVariant::fromValue<AbstractValue::Type>(this->type()).toString().toLower(); //Convert enum to lowercase string
    val["name"] = this->name();

    return val;
}

bool AbstractValue::loadFromJson(const QJsonObject &obj)
{
    //TODO Aaron: Move common stuff from subclasses here
}

