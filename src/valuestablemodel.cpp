#include "valuestablemodel.h"
#include "entry.h"
#include "abstractvalue.h"
#include "plainvalue.h"
#include "cryptedvalue.h"
#include <QDebug>
#include "database.h"

ValuesTableModel::ValuesTableModel() : mEntry(nullptr), mDatabase(nullptr)
{
}

ValuesTableModel::~ValuesTableModel()
{

}

void ValuesTableModel::setDatabase(Database *database) {
    mDatabase  = database;
}

void ValuesTableModel::setEntry(Entry *entry)
{
    beginResetModel();
    if(mEntry) {
        disconnect(mEntry,0,this,0);
    }
    mEntry = entry;
    if(mEntry) {
        connect(mEntry,&Entry::valueAdded,this,&ValuesTableModel::valueAdded);
        connect(mEntry,&Entry::valueRemoved,this,&ValuesTableModel::valueRemoved);
    }
    endResetModel();
}

int ValuesTableModel::rowCount(const QModelIndex &) const
{
    if(mEntry) {
        return mEntry->values().count();
    }
    return 0;
}

int ValuesTableModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant ValuesTableModel::data(const QModelIndex &index, int role) const
{
    if(mEntry) {
        AbstractValue* abstractValue = mEntry->values().at(index.row());
        if(role == Qt::UserRole) {
            return QVariant::fromValue(abstractValue);
        } else if(role == Qt::DisplayRole || role == Qt::EditRole) {
            int col = index.column();
            if(col == 0) {
                return abstractValue->name();
            } else if(col==1) {
                if(role == Qt::EditRole && (abstractValue->isEmpty() || abstractValue->type() != AbstractValue::Type::Plain)) {
                    return ""; // do not return real password in case its a crypted value. just let the user start with an empty one
                }
                return abstractValue->displayValue();
            }
        }
    }

    return QVariant();
}

QVariant ValuesTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role!=Qt::DisplayRole || orientation != Qt::Horizontal) {
        return QAbstractTableModel::headerData(section,orientation,role);
    }
    if(section == 0) {
        return "Title";
    } else if (section==1) {
        return "Value";
    }
    return QVariant();
}

void ValuesTableModel::valueAdded(int ind)
{
    beginInsertRows(QModelIndex(),ind,ind);
    endInsertRows();
}

void ValuesTableModel::valueRemoved(int ind)
{
    beginRemoveRows(QModelIndex(),ind,ind);
    endRemoveRows();
}


bool ValuesTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role != Qt::EditRole) {
        return false;
    }
    if(mEntry) {
        AbstractValue* abstractValue = mEntry->values().at(index.row());
        if(index.column() == 0) {
            abstractValue->setName(value.toString());
        } else if(index.column() == 1) {
            //Todo : Maybe change?
            if(abstractValue->type() == AbstractValue::Type::Plain) {
                PlainValue* plainValue = static_cast<PlainValue*>(abstractValue);
                plainValue->setValue(value.toString());
            } else if(abstractValue->type() == AbstractValue::Type::Encrypted){
                CryptedValue* cryptedValue = static_cast<CryptedValue*>(abstractValue);
                cryptedValue->setValue(mDatabase->protectedStreamKey(),value.toString());
            } else {
                qCritical() << "unknown value type";
            }
        }
        emit dataChanged(index,index,{role});
        return true;
    }
    return false;
}


Qt::ItemFlags ValuesTableModel::flags(const QModelIndex &) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}
