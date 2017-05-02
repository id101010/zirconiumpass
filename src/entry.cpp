#include "entry.h"

#include <QString>

Entry::Entry()
{

}

const QString &Entry::title()
{
    return mTitle;
}

void Entry::setTitle(const QString &title)
{
    mTitle = title;
}

QVector<AbstractValue *> Entry::values()
{

}

AbstractValue *Entry::valueByName(const QString &name)
{

}

void Entry::removeValue(const QString &name)
{

}

void Entry::addValue(AbstractValue *value)
{

}

QJsonObject Entry::saveToJson() const
{

}

bool Entry::loadFromJson(const QJsonObject &obj)
{

}
