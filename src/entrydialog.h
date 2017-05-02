#ifndef ENTRYDIALOG_H
#define ENTRYDIALOG_H

#include <QDialog>
#include "entry.h"

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

    // QDialog interface
public slots:
    virtual void accept() override;
    virtual int exec() override;
};

#endif // ENTRYDIALOG_H
