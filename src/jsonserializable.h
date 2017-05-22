#ifndef JSONSERIALIZABLE_H
#define JSONSERIALIZABLE_H

#include <QJsonObject>

/**
 * @brief The JsonSerializable interface declares the basic serialize/deserialize operations for Serializable objects
 */
class JsonSerializable
{
    public:
        virtual ~JsonSerializable() = default;
        virtual QJsonObject saveToJson() const = 0 ;
        virtual bool loadFromJson(const QJsonObject& obj) = 0;
};

#endif // JSONSERIALIZABLE_H
