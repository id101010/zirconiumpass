#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databaseopendialog.h"
#include "entrydialog.h"
#include <QAction>
#include <QTableView>
#include <QMenu>

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

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView,&QTableView::customContextMenuRequested,this,&MainWindow::tableContextMenuRequested);
    connect(ui->tableView,&QTableView::doubleClicked, this, &MainWindow::entryDoubleClicked);
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


//test stuff ahead
#include "entry.h"
#include "cryptedvalue.h"

void MainWindow::createNewDatabaseClicked()
{
    closeDatabaseClicked();
    mDatabase = Database::createNew("passw0rd");
    Entry* en = new Entry();
    en->setTitle("Eintrag 1");
    CryptedValue* cv = new CryptedValue();
    cv->setValue(mDatabase->protectedStreamKey(),"hans");
    cv->setName("passwort");
    en->addValue(cv);

    mDatabase->databaseContent().addEntry(en);
    mDatabase->write("test.db");
}

// end test stuff

void MainWindow::createNewEntryClicked()
{
    EntryDialog dialog (mDatabase.get());
    if(dialog.exec() == QDialog::Accepted) {
        mDatabase->databaseContent().addEntry(dialog.entry());
    }
}

void MainWindow::tableContextMenuRequested(const QPoint &pos)
{
    QModelIndex mInd =  ui->tableView->indexAt(pos);
    if(!mInd.isValid())  {
        return;
    }
    Entry* selectedEntry = mInd.data(Qt::UserRole).value<Entry*>(); //Get entry for index from model (by using custom role)
    if(selectedEntry == nullptr) {
        return;
    }

    QMenu m;
    QAction* editAction = m.addAction("Edit Entry");
    QAction* removeAction = m.addAction("Remove Entry");

    QAction* selectedAction = m.exec(ui->tableView->mapToGlobal(pos));
    if(selectedAction == editAction) {
        editEntry(selectedEntry);
    } else if(selectedAction == removeAction) {
        mDatabase->databaseContent().removeEntry(selectedEntry);
    }
}

void MainWindow::entryDoubleClicked(const QModelIndex &index)
{
    if(index.isValid()) {
        if(index.column() == 0) { //Title clicked
            editEntry(index.data(Qt::UserRole).value<Entry*>());
        }
    }
}

void MainWindow::editEntry(Entry *entry)
{
    EntryDialog dialog(mDatabase.get());
    dialog.setEntry(entry);
    if(dialog.exec() == QDialog::Accepted) {
        int row = mDatabase->databaseContent().entries().indexOf(entry);
        emit mEntriesModel.dataChanged(mEntriesModel.index(row,0),mEntriesModel.index(row,mEntriesModel.columnCount({})-1));
    }
}


