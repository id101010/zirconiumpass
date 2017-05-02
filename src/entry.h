#ifndef ENTRY_H
#define ENTRY_H

#include "abstractvalue.h"
#include "jsonserializable.h"

#include <QString>
#include <QVector>
#include <QMetaType>

class Entry : public JsonSerializable
{
    public:
        Entry();
        const QString& title();
        void setTitle(const QString &title);
        QVector<AbstractValue*> values();
        AbstractValue* valueByName(const QString& name);
        void removeValueByName(const QString& name);
        void addValue(AbstractValue* value);
        virtual QJsonObject saveToJson() const override;
        virtual bool loadFromJson(const QJsonObject &obj) override;
    private:
        QString mTitle;
        QVector<AbstractValue*> mValues;
};
Q_DECLARE_METATYPE(Entry*)

#endif // ENTRY_H
