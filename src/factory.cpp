#include "factory.h"
#include "plainvalue.h"
#include "cryptedvalue.h"
#include "entry.h"
#include <QDebug>

Factory::~Factory()
{

}

Entry *Factory::createEntry()
{
    return new Entry(this->sharedFromThis());
}


AbstractValue *Factory::createValue(AbstractValue::Type type)
{
    switch(type) {
        case AbstractValue::Type::encrypted:
            return new CryptedValue();
        case AbstractValue::Type::plain:
            return new PlainValue();
        default:
            qCritical() << "unknown type" << type;
            return nullptr;
    }
}
