#include <QFileDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QtDebug>
#include <QMessageBox>

#include "databaseopendialog.h"
#include "ui_databaseopendialog.h"
#include "database.h"

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

std::unique_ptr<Database> DatabaseOpenDialog::database()
{
    return std::move(mDatabase);
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
    if(ui->lblPassword->echoMode() == QLineEdit::Password){
        ui->lblPassword->setEchoMode(QLineEdit::Normal);
        ui->btnShow->setText("Hide");
    }else{
        ui->lblPassword->setEchoMode(QLineEdit::Password);
        ui->btnShow->setText("Show");
    }
}

void DatabaseOpenDialog::accept()
{
    // test if a database has been selected
    if(ui->lblDatabaseName->text() != ""){
        qDebug() << "Selected database: " << ui->lblDatabaseName->text();
    }else{
        qDebug() << "No database selected!";
        QMessageBox msgbox(this);
        msgbox.setWindowTitle("Error");
        msgbox.setText("No database selected!");
        msgbox.exec();
        return;
    }
    QString fileName = ui->lblDatabaseName->text();

    // test if a password has been provided
    if(ui->lblPassword->text() != ""){
        qDebug() << "Password: " << ui->lblPassword->text();
    }else{
        qDebug() << "No password!";
        QMessageBox msgbox(this);
        msgbox.setWindowTitle("Error");
        msgbox.setText("No password!");
        msgbox.exec();
        return;
    }

    mDatabase = std::unique_ptr<Database>(Database::createFromFile(fileName));

    close();
}
