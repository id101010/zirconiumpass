#ifndef ABSTRACTVALUE_H
#define ABSTRACTVALUE_H

#include "jsonserializable.h"
#include <QMetaType>

class AbstractValue : public JsonSerializable
{
    private:
        QString mName;
    public:
        AbstractValue();
        const QString& name() const;
        void setName(const QString& name);
        QJsonObject saveToJson() const override;
        bool loadFromJson(const QJsonObject &obj) override;
        virtual const QString& type(void) const = 0;
};
Q_DECLARE_METATYPE(AbstractValue*)

#endif // ABSTRACTVALUE_H
