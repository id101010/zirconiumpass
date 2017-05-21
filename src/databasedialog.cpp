#include <QFileDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QtDebug>
#include <QMessageBox>
#include <QSettings>

#include "databasedialog.h"
#include "ui_databasedialog.h"
#include "database.h"

DatabaseDialog::DatabaseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatabaseDialog), mMode(Mode::Open)
{
    ui->setupUi(this);
}

DatabaseDialog::~DatabaseDialog()
{
    delete ui;
}

std::unique_ptr<Database> DatabaseDialog::database()
{
    return std::move(mDatabase);
}

void DatabaseDialog::setMode(DatabaseDialog::Mode m)
{
    mMode = m;
}

QString DatabaseDialog::filename() const
{
    return mFilename;
}

void DatabaseDialog::on_btnDatabase_clicked()
{
    QFileDialog filedialog;
    QStringList fileNames;
    filedialog.setDefaultSuffix(".zdb");
    filedialog.setNameFilter("Zirconiumpass Database Files (*.zdb)");

    if(mMode == Mode::Open) {
        filedialog.setFileMode(QFileDialog::ExistingFile);
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
    } else {
        filedialog.selectFile("main.zdb");
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
    }

    if (filedialog.exec()){
        fileNames = filedialog.selectedFiles();
        ui->lblDatabaseName->setText(fileNames[0]);
        ui->lblPassword->setFocus();
    }
}

void DatabaseDialog::on_btnShow_clicked()
{
    if(ui->lblPassword->echoMode() == QLineEdit::Password){
        ui->lblPassword->setEchoMode(QLineEdit::Normal);
        ui->btnShow->setText("Hide");
    }else{
        ui->lblPassword->setEchoMode(QLineEdit::Password);
        ui->btnShow->setText("Show");
    }
}

void DatabaseDialog::accept()
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

    if(mMode == Mode::Open) { //open existing
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
        } else {
            qDebug() << "Password ok";
        }
    } else { //create new
        mDatabase = std::unique_ptr<Database>(Database::createNew(password));
    }

    password.fill('0',password.length()); //Overwrite password memory region
    ui->lblPassword->setText(password); //Also remove password in gui class

    //Save last used database path
    QSettings settings;
    settings.setValue("lastdatabasepath",fileName);

    mFilename = fileName;
    QDialog::accept();
}


int DatabaseDialog::exec()
{
    if(mMode == Mode::Open) {

        QSettings settings;
        QString path = settings.value("lastdatabasepath","").toString();

        ui->lblDatabaseName->setText(path);
        if(!path.isEmpty()) {
            ui->lblPassword->setFocus();
        }

        setWindowTitle("Open Database");
    } else {
        setWindowTitle("Create new database");
        on_btnDatabase_clicked();
    }
    return QDialog::exec();
}
