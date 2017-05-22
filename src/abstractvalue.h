#ifndef ABSTRACTVALUE_H
#define ABSTRACTVALUE_H

#include "jsonserializable.h"
#include <QMetaType>

/**
 * @brief The AbstractValue class stores one value of an entry (e.g  a password or a note)
 */
class AbstractValue : public JsonSerializable
{
    Q_GADGET //needed for Q_ENUM
    private:
        QString mName;
    public:
        enum class Type {
            encrypted,
            plain
        };
        Q_ENUM(Type)

        AbstractValue();
        const QString& name() const;
        void setName(const QString& name);
        QJsonObject saveToJson() const override;
        bool loadFromJson(const QJsonObject &obj) override;
        virtual Type type() const = 0;
        virtual QString displayValue() const = 0;
        virtual bool isEmpty() const = 0;
};
Q_DECLARE_METATYPE(AbstractValue*)

#endif // ABSTRACTVALUE_H
