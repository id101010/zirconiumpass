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
        void removeValue(const QString& name);
        void addValue(AbstractValue* value);
        QJsonObject saveToJson() const override;
        bool loadFromJson(const QJsonObject &obj) override;
    private:
        QString mTitle;
};
Q_DECLARE_METATYPE(Entry*)

#endif // ENTRY_H
