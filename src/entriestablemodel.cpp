#include "entriestablemodel.h"
#include "database.h"
#include "entry.h"
#include "abstractvalue.h"
#include "plainvalue.h"
#include <QSettings>

EntriesTableModel::EntriesTableModel() : mDatabase(nullptr)
{
    QSettings settings;
    mAdditionalColumns =  settings.value("maincolumns", (QStringList() << "password0" << "url" )).toStringList();
}

EntriesTableModel::~EntriesTableModel()
{

}

void EntriesTableModel::setDatabase(const Database *database)
{
    beginResetModel();
    if(mDatabase) {
        disconnect(&(mDatabase->databaseContent()),0,this,0);
    }
    mDatabase = database;
    if(mDatabase) {
        connect(&(mDatabase->databaseContent()),&DatabaseContent::entryAdded,this,&EntriesTableModel::entryAdded);
        connect(&(mDatabase->databaseContent()),&DatabaseContent::entryRemoved,this,&EntriesTableModel::entryRemoved);
    }
    endResetModel();
}

int EntriesTableModel::rowCount(const QModelIndex &) const
{
    if(mDatabase) {
        return mDatabase->databaseContent().entries().count();
    }
    return 0;
}

int EntriesTableModel::columnCount(const QModelIndex &) const
{
    return 1 + mAdditionalColumns.count();
}

QVariant EntriesTableModel::data(const QModelIndex &index, int role) const
{
    //We implement the following behaviour:
    // If the user requests the DisplayRole: Show him the title, the display value or "<no such value>"
    // if the user requests UserRole: Return him the Entry (for the 'title' column) or the AbtractValue (for any 'value' column)


    if(mDatabase) {
        Entry* entry = mDatabase->databaseContent().entries().at(index.row());
        int col = index.column();
        if(col == 0) { // title column
            if(role == Qt::UserRole) {
                return QVariant::fromValue(entry);
            } else if(role == Qt::DisplayRole) {
                return entry->title();
            }
        } else { // value column
           AbstractValue* value = entry->valueByName(mAdditionalColumns[col-1]);
           if(value != nullptr) {
               if(role == Qt::UserRole) {
                   return QVariant::fromValue(value);
               } else if(role == Qt::DisplayRole) {
                    return value->displayValue();
               }
           } else {
               if(role == Qt::UserRole) {
                   return QVariant();
               } else if(role == Qt::DisplayRole) {
                  return "<no such value>";
               }

           }
        }
    }

    return QVariant();
}

QVariant EntriesTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role!=Qt::DisplayRole || orientation != Qt::Horizontal) {
        return QAbstractTableModel::headerData(section,orientation,role);
    }
    if(section == 0) {
        return "Title";
    } else {
        return mAdditionalColumns[section-1];
    }
}

void EntriesTableModel::removeColumnAt(int ind)
{
    if(ind>0 && ind < columnCount({})) {
        beginRemoveColumns({},ind,ind);
        mAdditionalColumns.removeAt(ind-1);
        endRemoveColumns();

        QSettings settings;
        settings.setValue("maincolumns", mAdditionalColumns);
    }
}

void EntriesTableModel::addColumn(const QString &key)
{
    beginInsertColumns({},columnCount({}),columnCount({}));
    mAdditionalColumns.append(key);
    endInsertColumns();

    QSettings settings;
    settings.setValue("maincolumns", mAdditionalColumns);
}

void EntriesTableModel::entryAdded(int ind)
{
    beginInsertRows({},ind,ind);
    endInsertRows();
}

void EntriesTableModel::entryRemoved(int ind)
{
    beginRemoveRows({},ind,ind);
    endRemoveRows();
}
