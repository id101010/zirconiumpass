#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databaseopendialog.h"
#include <QAction>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->actionClose->setEnabled(false);
    ui->actionCreateNewEntry->setEnabled(false);
    ui->actionEdit_Entry->setEnabled(false);
    ui->actionDelete_Entry->setEnabled(false);
    ui->tableView->setModel(&mEntriesModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openDatabaseClicked()
{
    DatabaseOpenDialog dialog;
    if(dialog.exec() == QDialog::Accepted) {
        mDatabase = dialog.database();
        if(mDatabase) {
            ui->actionClose->setEnabled(true);
            ui->actionCreateNewEntry->setEnabled(true);
            mEntriesModel.setDatabase(mDatabase.get());
        }
    }
}

void MainWindow::closeDatabaseClicked()
{
    if(mDatabase) {
        mEntriesModel.setDatabase(nullptr);
        mDatabase.reset();
        ui->actionClose->setEnabled(false);
    }
}

void MainWindow::createNewDatabaseClicked()
{

}


