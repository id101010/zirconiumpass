#ifndef JSONSERIALIZABLE_H
#define JSONSERIALIZABLE_H
#include <QJsonObject>


class JsonSerializable {
public:
    virtual ~JsonSerializable() = 0;
    virtual QJsonObject saveToJson() const = 0 ;
    virtual bool loadFromJson(const QJsonObject& obj) = 0;

};

#endif // JSONSERIALIZABLE_H
