#include <QFileDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QtDebug>
#include <QMessageBox>
#include <QSettings>

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
        ui->lblPassword->setFocus();
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
    QString password = ui->lblPassword->text();

    mDatabase = std::unique_ptr<Database>(Database::createFromFile(fileName));
    if(!mDatabase) {
        qDebug() << "File open error";
        QMessageBox::warning(this,"Database open Error","The database file could not be opened");
        return;
    }

    if(!mDatabase->decrypt(password)) {
        qDebug() << "Password wrong";
        QMessageBox::warning(this,"Database open Error","Wrong password?");
        ui->lblPassword->clear();
        return;
    }

    password.fill('0',password.length()); //Overwrite password memory region
    ui->lblPassword->setText(password); //Also remove password in gui class

    //Save last used database path
    QSettings settings;
    settings.setValue("lastdatabasepath",fileName);

    qDebug() << "Password ok";
    QDialog::accept();
}


int DatabaseOpenDialog::exec()
{
    QSettings settings;
    QString path = settings.value("lastdatabasepath","").toString();
    ui->lblDatabaseName->setText(path);
    if(!path.isEmpty()) {
        ui->lblPassword->setFocus();
    }
    return QDialog::exec();
}
