#include "entry.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QString>
#include <QDebug>

Entry::Entry()
{

}

const QString &Entry::title()
{
    return mTitle;
}

void Entry::setTitle(const QString &title)
{
    mTitle = title;
}

QVector<AbstractValue *> Entry::values()
{
    return mValues;
}

AbstractValue *Entry::valueByName(const QString &name)
{
    for(int i = 0; i < mValues.size(); i++){
        if(mValues[i]->name() == name){
            return mValues[i];
        }
    }

    return nullptr;
}

void Entry::removeValueByName(const QString &name)
{
    AbstractValue *objectToRemove = valueByName(name);

    if(objectToRemove != nullptr){
        mValues.removeOne(objectToRemove);
    }
}

void Entry::addValue(AbstractValue *value)
{
    mValues.append(value);
}

QJsonObject Entry::saveToJson() const
{
    QJsonObject entry; // root object
    QJsonArray jarray; // array which is the container for all values

    entry.insert("title", mTitle);

    // Save all values
    for(int i = 0; i < mValues.size(); i++){
        jarray.append(mValues[i]->saveToJson());
    }

    entry["values"] = jarray;

    //Debug print
    QJsonDocument doc;
    doc.setObject(entry);
    qDebug() << doc.toJson();

    return entry;
}

bool Entry::loadFromJson(const QJsonObject &obj)
{

}
