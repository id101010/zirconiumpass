#ifndef ENTRYDIALOG_H
#define ENTRYDIALOG_H

#include <QDialog>
#include "entry.h"
#include "valuestablemodel.h"

namespace Ui {
class EntryDialog;
}

class EntryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EntryDialog(class Database *database);
    Entry* entry();
    void setEntry(Entry* entry);
    ~EntryDialog();

private:
    Ui::EntryDialog *ui;
    Entry* mEntry;
    ValuesTableModel mValuesTableModel;
    class Database* mDatabase;

    // QDialog interface
public slots:
    virtual void accept() override;
    virtual int exec() override;
private slots:
    void on_pbNewPlain_clicked();
    void on_pbNewCrypted_clicked();
    void tableContextMenuRequested(const QPoint &pos);
};

#endif // ENTRYDIALOG_H
