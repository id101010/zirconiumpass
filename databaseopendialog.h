#ifndef DATABASEOPENDIALOG_H
#define DATABASEOPENDIALOG_H

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

private slots:
    void on_btnDatabase_clicked();
    void on_btnShow_clicked();

private:
    Ui::DatabaseOpenDialog *ui;

    // QDialog interface
public slots:
    void accept();
};

#endif // DATABASEOPENDIALOG_H
