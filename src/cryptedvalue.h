#ifndef CRYPTEDVALUE_H
#define CRYPTEDVALUE_H

#include "abstractvalue.h"

#include <QByteArray>
#include <functional>

/**
 * @brief The CryptedValue class stores a encrypted AbstractValue
 */
class CryptedValue : public AbstractValue
{
    public:
        CryptedValue();
        void decrypt(const QByteArray& streamkey, std::function<void(const QString &)> visitor);
        void setValue(const QByteArray& streamkey, const QString& value);
        virtual QJsonObject saveToJson() const override;
        virtual bool loadFromJson(const QJsonObject &obj) override;
        virtual AbstractValue::Type type() const override;
        virtual QString displayValue() const override;
        virtual bool isEmpty() const override;

    private:
        QByteArray mValue;

};

#endif // CRYPTEDVALUE_H
