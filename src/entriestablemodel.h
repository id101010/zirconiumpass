#ifndef ENTRIESTABLEMODEL_H
#define ENTRIESTABLEMODEL_H

#include <QAbstractTableModel>
class EntriesTableModel : public QAbstractTableModel
{
public:
    EntriesTableModel();
    virtual ~EntriesTableModel();

    void setDatabase(const class Database* database);


    virtual int rowCount(const QModelIndex &parent) const override;

    virtual int columnCount(const QModelIndex &parent) const override;

    virtual QVariant data(const QModelIndex &index, int role) const override;


    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
private slots:
    void entryAdded(int ind);
    void entryRemoved(int ind);
private:
    const class Database* mDatabase;
    QStringList mAdditionalColumns;

};

#endif // ENTRIESTABLEMODEL_H
