#ifndef ENTRY_H
#define ENTRY_H

#include "abstractvalue.h"
#include "jsonserializable.h"

#include <QString>
#include <QVector>
#include <QMetaType>
#include <QObject>

class Entry : public QObject, public JsonSerializable
{
    Q_OBJECT
    public:
        Entry();
        const QString& title();
        void setTitle(const QString &title);
        QVector<AbstractValue*> values();
        AbstractValue* valueByName(const QString& name);
        void removeValueByName(const QString& name);
        void removeValue(AbstractValue* value);
        void addValue(AbstractValue* value);
        virtual QJsonObject saveToJson() const override;
        virtual bool loadFromJson(const QJsonObject &obj) override;
    private:
        QString mTitle;
        QVector<AbstractValue*> mValues;
    signals:
        void valueAdded(int index);
        void valueRemoved(int index);
};
Q_DECLARE_METATYPE(Entry*)

#endif // ENTRY_H
