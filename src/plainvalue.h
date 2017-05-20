#ifndef PLAINVALUE_H
#define PLAINVALUE_H

#include "abstractvalue.h"

class PlainValue : public AbstractValue
{
    public:
        PlainValue();
        const QString& value();
        void setValue(const QString& value);
        virtual QJsonObject saveToJson() const override;
        virtual bool loadFromJson(const QJsonObject &obj) override;
        virtual const QString &type() const override;
        virtual QString displayValue() const override;
        virtual bool isEmpty() const override;
    private:
        QString mValue;



};

#endif // PLAINVALUE_H
