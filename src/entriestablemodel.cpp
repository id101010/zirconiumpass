#include "entriestablemodel.h"
#include "database.h"

EntriesTableModel::EntriesTableModel() : mDatabase(nullptr)
{

}

EntriesTableModel::~EntriesTableModel()
{

}

void EntriesTableModel::setDatabase(const Database *database)
{
    mDatabase = database;
}

int EntriesTableModel::rowCount(const QModelIndex &parent) const
{
    if(mDatabase) {
        return mDatabase->databaseContent().entries().count();
    }
    return 0;
}

int EntriesTableModel::columnCount(const QModelIndex &parent) const
{
    return 1 + mAdditionalColumns.count();
}

QVariant EntriesTableModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::DisplayRole) {
        return QVariant();
    }
    if(mDatabase) {
          /* Entry* entry = mDatabase->databaseContent().entries().at(index.row());
           int col = index.column();
           if(col == 0) {
               return entry->title();
           } else {
               AbstractValue* value = entry->valueByName(mAdditionalColumns[col-1]);
           }*/

    }

    return QVariant();
}

QVariant EntriesTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(section == 0) {
        return "Title";
    } else {
        return mAdditionalColumns[section-1];
    }
}
