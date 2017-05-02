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

void MainWindow::createNewDatabaseClicked()
{
    closeDatabaseClicked();
    mDatabase = Database::createNew("passw0rd");
    Entry* en = new Entry();
    en->setTitle("Eintrag 1");

    mDatabase->databaseContent().addEntry(en);
    mDatabase->write("test.db");
}

// end test stuff

void MainWindow::createNewEntryClicked()
{
    EntryDialog dialog;
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
    Entry* selectedEntry = mInd.data(Qt::UserRole).value<Entry*>();
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

void MainWindow::editEntry(Entry *entry)
{
    EntryDialog dialog;
    dialog.setEntry(entry);
    if(dialog.exec() == QDialog::Accepted) {
        int row = mDatabase->databaseContent().entries().indexOf(entry);
        emit mEntriesModel.dataChanged(mEntriesModel.index(row,0),mEntriesModel.index(row,mEntriesModel.columnCount({})-1));
    }
}


