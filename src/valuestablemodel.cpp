#include "valuestablemodel.h"
#include "entry.h"
#include "abstractvalue.h"
#include "plainvalue.h"

ValuesTableModel::ValuesTableModel() : mEntry(nullptr)
{
}

ValuesTableModel::~ValuesTableModel()
{

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
        AbstractValue* value = mEntry->values().at(index.row());
        if(role == Qt::UserRole) {
            return QVariant::fromValue(value);
        } else if(role == Qt::DisplayRole) {
            int col = index.column();
            if(col == 0) {
               return value->name();
            } else {
               return QVariant();
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
