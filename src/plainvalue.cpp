#include "plainvalue.h"

PlainValue::PlainValue()
{

}

/**
 * @brief Returns the currently stored value
 * @return
 */
const QString &PlainValue::value()
{
    return mValue;
}

/**
 * @brief Replaces the currently stored value
 * @param value
 */
void PlainValue::setValue(const QString &value)
{
    this->mValue = value;
}

/**
 * @brief Serializes the instance to a JSON Object
 * @return
 */
QJsonObject PlainValue::saveToJson() const
{
    QJsonObject val = AbstractValue::saveToJson();
    val["plainValue"] = mValue;
    return val;
}

/**
 * @brief Restores the value from a jsonobject
 * @param obj
 * @return True on success
 */
bool PlainValue::loadFromJson(const QJsonObject &obj)
{
    if(!AbstractValue::loadFromJson(obj)) {
        return false;
    }

    /* try to load plainValue */
    if(obj["plainValue"].isString()){
        mValue = obj["plainValue"].toString();
    } else {
        return false;
    }

    return true;
}


/**
 * @brief Returns the type of this object
 */
AbstractValue::Type PlainValue::type() const
{
    return AbstractValue::Type::plain;
}


/**
 * @brief Returns true if the plain value is empty
 */
bool PlainValue::isEmpty() const
{
    return mValue.isEmpty();
}


/**
 * @brief Returns a display representation of the value.
 */
QString PlainValue::displayValue() const
{
    if(isEmpty()) {
        return "<emtpy>";
    } else {
        return mValue;
    }
}

