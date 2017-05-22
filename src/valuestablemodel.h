#ifndef VALUESTABLEMODEL_H
#define VALUESTABLEMODEL_H

#include <QAbstractTableModel>



/**
 * @brief The ValuesTableModel class Visualizes a list of AbstractValues (belonging to the same Entry)
 */
class ValuesTableModel : public QAbstractTableModel
{
public:
    ValuesTableModel();
    virtual ~ValuesTableModel();

    void setEntry(class Entry* entry);
    void setDatabase(class Database* database);


    virtual int rowCount(const QModelIndex &parent) const override;

    virtual int columnCount(const QModelIndex &parent) const override;

    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
private slots:
    void valueAdded(int ind);
    void valueRemoved(int ind);
private:
    class Entry* mEntry;
    class Database* mDatabase;



};

#endif // VALUESTABLEMODEL_H
