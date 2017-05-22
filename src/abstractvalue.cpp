#include "abstractvalue.h"
#include <QMetaEnum>
#include <QVariant>

/**
 * @brief Constructor for this class.
 */
AbstractValue::AbstractValue()
{

}

/**
 * @brief Returns the name of this abstract value.
 */
const QString &AbstractValue::name() const
{
    return mName;
}

/**
 * @brief Sets the name of this abstract value.
 * @param name
 */
void AbstractValue::setName(const QString &name)
{
    mName = name;
}

/**
 * @brief Generate a JSON Object from abstract value
 */
QJsonObject AbstractValue::saveToJson() const
{
    QJsonObject val;
    val["type"] = QVariant::fromValue<AbstractValue::Type>(this->type()).toString(); //Convert enum to string
    val["name"] = this->name();

    return val;
}

/**
 * @brief Load an abstract vale from a JSON Object
 * @param JSON Object
 */
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
