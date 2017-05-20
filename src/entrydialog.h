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
    explicit EntryDialog(QWidget *parent = 0);
    Entry* entry();
    void setEntry(Entry* entry);
    ~EntryDialog();

private:
    Ui::EntryDialog *ui;
    Entry* mEntry;
    ValuesTableModel mValuesTableModel;

    // QDialog interface
public slots:
    virtual void accept() override;
    virtual int exec() override;
private slots:
    void on_pushButton_clicked();
};

#endif // ENTRYDIALOG_H
