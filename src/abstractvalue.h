#ifndef ABSTRACTVALUE_H
#define ABSTRACTVALUE_H

#include "jsonserializable.h"

class AbstractValue : public JsonSerializable
{
    private:
        QString mName;
    public:
        AbstractValue();
        const QString& name();
        void setName(const QString& name);
        QJsonObject saveToJson() const override;
        bool loadFromJson(const QJsonObject &obj) override;
};

#endif // ABSTRACTVALUE_H
