#include <QFileDialog>
#include <QLineEdit>
#include "databaseopendialog.h"
#include "ui_databaseopendialog.h"

DatabaseOpenDialog::DatabaseOpenDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatabaseOpenDialog)
{
    ui->setupUi(this);
}

DatabaseOpenDialog::~DatabaseOpenDialog()
{
    delete ui;
}

void DatabaseOpenDialog::on_btnDatabase_clicked()
{
    QFileDialog filedialog;
    QStringList fileNames;

    if (filedialog.exec()){
        fileNames = filedialog.selectedFiles();
        ui->lblDatabaseName->setText(fileNames[0]);
    }
}

void DatabaseOpenDialog::on_btnShow_clicked()
{

}
