#ifndef DATABASEOPENDIALOG_H
#define DATABASEOPENDIALOG_H
#include "database.h"
#include <memory>

#include <QDialog>

namespace Ui {
class DatabaseDialog;
}

class DatabaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DatabaseDialog(QWidget *parent = 0);
    ~DatabaseDialog();
    std::unique_ptr<Database> database();


    enum class Mode {
        Open, CreateNew
    };

    void setMode(Mode m);
    QString filename() const;



private slots:
    void on_btnDatabase_clicked();
    void on_btnShow_clicked();

private:
    Ui::DatabaseDialog *ui;
    std::unique_ptr<Database> mDatabase;
    Mode mMode;
    QString mFilename;

    // QDialog interface
public slots:
    virtual void accept() override;
    virtual int exec() override;
};

#endif // DATABASEOPENDIALOG_H
