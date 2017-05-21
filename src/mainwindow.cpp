#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databaseopendialog.h"
#include "entrydialog.h"
#include <QAction>
#include <QTableView>
#include <QMenu>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->actionClose->setEnabled(false);
    ui->actionCreateNewEntry->setEnabled(false);
    ui->actionEdit_Entry->setEnabled(false);
    ui->actionDelete_Entry->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->tableView->setModel(&mEntriesModel);

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView,&QTableView::customContextMenuRequested,this,&MainWindow::tableContextMenuRequested);
    connect(ui->tableView,&QTableView::doubleClicked, this, &MainWindow::entryDoubleClicked);

    ui->tableView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView->horizontalHeader(), &QHeaderView::customContextMenuRequested,this,&MainWindow::tableHeaderContextMenuRequested);
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
            ui->actionSave->setEnabled(true);
            ui->actionCreateNewEntry->setEnabled(true);
            mEntriesModel.setDatabase(mDatabase.get());

            //Todo: store database path to member variable, so that we can use in the save function
        }
    }
}

void MainWindow::closeDatabaseClicked()
{
    if(mDatabase) {
        mEntriesModel.setDatabase(nullptr);
        mDatabase.reset();
        ui->actionClose->setEnabled(false);
        ui->actionSave->setEnabled(false);
        ui->actionCreateNewEntry->setEnabled(false);
    }
}


void MainWindow::createNewDatabaseClicked()
{
    closeDatabaseClicked();
    ui->actionClose->setEnabled(true);
    ui->actionCreateNewEntry->setEnabled(true);
    ui->actionSave->setEnabled(true);
    mDatabase = Database::createNew("passw0rd"); //TODO: Use databaseopen dialog to enter a initial password and path (save path to member variable)
}


void MainWindow::createNewEntryClicked()
{
    EntryDialog dialog (mDatabase.get());
    if(dialog.exec() == QDialog::Accepted) {
        mDatabase->databaseContent().addEntry(dialog.entry());
    }
}

void MainWindow::saveDatabaseClicked()
{
    if(mDatabase) {
        mDatabase->write("test.db"); //TODO: use dynamic save path
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

void MainWindow::tableHeaderContextMenuRequested(const QPoint &pos)
{
    int column=ui->tableView->horizontalHeader()->logicalIndexAt(pos);

    QMenu m;
    QAction* removeAction = m.addAction("Remove Column");
    QAction* addAction = m.addAction("Add new Column....");


    if(column <= 0 || column >= mEntriesModel.columnCount({})) {
        removeAction->setEnabled(false);
    }

    QAction* selectedAction = m.exec(ui->tableView->horizontalHeader()->mapToGlobal(pos));
    if(selectedAction == removeAction) {
        mEntriesModel.removeColumnAt(column);
    } else if(selectedAction == addAction) {
        QString valueName = QInputDialog::getText(this,"Add column", "Value name");
        if(!valueName.trimmed().isEmpty()) {
            mEntriesModel.addColumn(valueName);
        }
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


