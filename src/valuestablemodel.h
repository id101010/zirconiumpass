#ifndef VALUESTABLEMODEL_H
#define VALUESTABLEMODEL_H

#include <QAbstractTableModel>
class ValuesTableModel : public QAbstractTableModel
{
public:
    ValuesTableModel();
    virtual ~ValuesTableModel();

    void setEntry(class Entry* entry);


    virtual int rowCount(const QModelIndex &parent) const override;

    virtual int columnCount(const QModelIndex &parent) const override;

    virtual QVariant data(const QModelIndex &index, int role) const override;


    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
private slots:
    void valueAdded(int ind);
    void valueRemoved(int ind);
private:
    class Entry* mEntry;


};

#endif // VALUESTABLEMODEL_H
