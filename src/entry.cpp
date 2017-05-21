#include "entry.h"
#include "plainvalue.h"
#include "cryptedvalue.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QString>
#include <QDebug>
#include <QMetaEnum>

Entry::Entry(QSharedPointer<Factory> factory) : mFactory(factory)
{

}

Entry::~Entry()
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
        int ind = mValues.indexOf(objectToRemove);
        mValues.removeAt(ind);
        emit valueRemoved(ind);
    }
}

void Entry::removeValue(AbstractValue *value)
{
    if(value != nullptr){
        int ind = mValues.indexOf(value);
        if(ind != -1) {
            mValues.removeAt(ind);
            emit valueRemoved(ind);
        }
    }
}

void Entry::addValue(AbstractValue *value)
{
    Q_ASSERT(value!=nullptr);
    mValues.append(value);
    emit valueAdded(mValues.size()-1);
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
    /* Load title from json */
    if(obj["title"].isString()){
        mTitle = obj["title"].toString();
    } else {
        return false;
    }

    QVector<AbstractValue*> vec;

    /* Load value array from json */
    if(obj["values"].isArray()){
        QJsonArray arr = obj["values"].toArray();
        for(int i=0; i<arr.size(); i++){
            if(arr.at(i).isObject()){
                QJsonObject tmp = arr.at(i).toObject();

                //Convert type string into enum using QMetaEnum reflection magic
                QMetaEnum me = QMetaEnum::fromType<AbstractValue::Type>();
                bool castOk = false;
                AbstractValue::Type type = static_cast<AbstractValue::Type>(me.keyToValue(tmp["type"].toString().toStdString().c_str(),&castOk));
                if(!castOk) {
                    return false;
                }

                //Create correct AbstractValue subclass via factory
                AbstractValue *v = mFactory->createValue(type);
                if(v == nullptr) {
                    return false;
                }

                /* load from json */
                if(v->loadFromJson(tmp)){
                    vec.append(v);
                } else {
                    return false;
                }

            } else {
                return false;
            }
        }
    } else {
        return false;
    }

    /* load to local mValues */
    mValues = vec;

    return true;
}
