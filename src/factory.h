#ifndef FACTORY_H
#define FACTORY_H


#include <QEnableSharedFromThis>
#include "abstractvalue.h"


/**
 * @brief The Factory struct is used internally to faciltate testing of core data classes
 */
struct Factory : public QEnableSharedFromThis<Factory> {
    virtual ~Factory();
    virtual class Entry* createEntry();
    virtual AbstractValue* createValue(AbstractValue::Type type);
};


#endif // FACTORY_H
