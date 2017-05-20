#ifndef DATABASEOPENDIALOG_H
#define DATABASEOPENDIALOG_H
#include "database.h"
#include <memory>

#include <QDialog>

namespace Ui {
class DatabaseOpenDialog;
}

class DatabaseOpenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DatabaseOpenDialog(QWidget *parent = 0);
    ~DatabaseOpenDialog();
    std::unique_ptr<Database> database();

private slots:
    void on_btnDatabase_clicked();
    void on_btnShow_clicked();

private:
    Ui::DatabaseOpenDialog *ui;
    std::unique_ptr<Database> mDatabase;

    // QDialog interface
public slots:
    virtual void accept() override;
    virtual int exec() override;
};

#endif // DATABASEOPENDIALOG_H
