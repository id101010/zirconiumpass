#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databaseopendialog.h"
#include "entrydialog.h"
#include <QAction>
#include <QTableView>
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), unsavedChanges(false)
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

    connect(ui->tableView->selectionModel(),&QItemSelectionModel::selectionChanged, this, &MainWindow::selectionChanged);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openDatabaseClicked()
{
    if(closeDatabaseClicked()) { //closedatabase was cancelled
        return;
    }
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

bool MainWindow::closeDatabaseClicked()
{
    if(mDatabase) {

        if(unsavedChanges) {
            int result = QMessageBox::warning(this, "Unsaved changes", "You have unsaved changes. Do you want to save the database before closing it?", QMessageBox::Yes| QMessageBox::No | QMessageBox::Cancel,QMessageBox::Yes);
            if(result == QMessageBox::Yes) { //save
                saveDatabaseClicked();
            } else if (result == QMessageBox::No) { //discard
               // no save here, close code below
            } else if (result == QMessageBox::Cancel) {
                return true; //close cancelled
            }
        }

        //Closing database
        mEntriesModel.setDatabase(nullptr);
        mDatabase.reset();
        ui->actionClose->setEnabled(false);
        ui->actionSave->setEnabled(false);
        ui->actionEdit_Entry->setEnabled(false);
        ui->actionDelete_Entry->setEnabled(false);
        ui->actionCreateNewEntry->setEnabled(false);
        unsavedChanges = false;
    }
    return false; //close was not canceled
}


void MainWindow::createNewDatabaseClicked()
{
    if(closeDatabaseClicked()) { //closedatabase was cancelled
        return;
    }
    ui->actionClose->setEnabled(true);
    ui->actionCreateNewEntry->setEnabled(true);
    ui->actionSave->setEnabled(true);

    //TODO: Use databaseopen dialog to enter a initial password and path (save path to member variable)
    mDatabase = Database::createNew("passw0rd");
    QMessageBox::information(this,"New database created", "A new database has been created. Start creating an entry. And press CTRL+S to save it to test.db",QMessageBox::Ok);


    mEntriesModel.setDatabase(mDatabase.get());
    unsavedChanges = true;
}


void MainWindow::createNewEntryClicked()
{
    EntryDialog dialog (mDatabase.get());
    if(dialog.exec() == QDialog::Accepted) {
        mDatabase->databaseContent().addEntry(dialog.entry());
        unsavedChanges = true;
    }
}

void MainWindow::saveDatabaseClicked()
{
    if(mDatabase) {
        mDatabase->write("test.db"); //TODO: use dynamic save path
        unsavedChanges = false;
    }
}

void MainWindow::editEntryClicked()
{
    Entry* selectedEntry = mEntriesModel.data(mEntriesModel.index(ui->tableView->currentIndex().row(),0,{}),Qt::UserRole).value<Entry*>();
    editEntry(selectedEntry);
}

void MainWindow::deleteEntryClicked()
{
     Entry* selectedEntry = mEntriesModel.data(mEntriesModel.index(ui->tableView->currentIndex().row(),0,{}),Qt::UserRole).value<Entry*>();
     deleteEntry(selectedEntry);
}


void MainWindow::tableContextMenuRequested(const QPoint &pos)
{
    QModelIndex mInd =  ui->tableView->indexAt(pos);
    if(!mInd.isValid())  {
        return;
    }

    int column = mInd.column();
    if(column <0 || column >= mEntriesModel.columnCount({})) {
            return;
    }


    QMenu m;
    QAction* copyAction = nullptr;
    AbstractValue* selectedValue = nullptr;

    if(column != 0) { //not title column
        copyAction = m.addAction("Copy "+mEntriesModel.headerData(column,Qt::Horizontal,Qt::DisplayRole).toString());
        QFont f = copyAction->font();
        f.setBold(true);
        copyAction->setFont(f);

        selectedValue = mInd.data(Qt::UserRole).value<AbstractValue*>();
        if(selectedValue == nullptr) {
            copyAction->setEnabled(false);
        }
    }


    QAction* editAction = m.addAction("Edit Entry...");
    QAction* removeAction = m.addAction("Remove Entry");
    if(column== 0) { //title column
        QFont f = editAction->font();
        f.setBold(true);
        editAction->setFont(f);
    }


    QAction* selectedAction = m.exec(ui->tableView->mapToGlobal(pos));
    if(selectedAction == nullptr) {
        return;
    }

     //Get entry be requesting UserRole data from Title cell
     Entry* selectedEntry = mEntriesModel.data(mEntriesModel.index(mInd.row(),0,{}),Qt::UserRole).value<Entry*>();

    if(selectedAction == editAction) {
        editEntry(selectedEntry);
    } else if(selectedAction == removeAction) {
        deleteEntry(selectedEntry);
    } else if(selectedAction == copyAction ) {
        EntryDialog::copyToClipboard(selectedValue,mDatabase.get());
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

void MainWindow::selectionChanged(const QItemSelection &selected)
{
    bool someEntriesSelected = !selected.empty();
    ui->actionEdit_Entry->setEnabled(someEntriesSelected);
    ui->actionDelete_Entry->setEnabled(someEntriesSelected);
}



void MainWindow::entryDoubleClicked(const QModelIndex &index)
{
    if(index.isValid()) {
        if(index.column() == 0) { //Title clicked
            editEntry(index.data(Qt::UserRole).value<Entry*>());
        } else { //value clicked
            AbstractValue* value = index.data(Qt::UserRole).value<AbstractValue*>();
            if(value!=nullptr) {
                 EntryDialog::copyToClipboard(value,mDatabase.get());
            }
        }
    }
}


void MainWindow::editEntry(Entry *entry)
{
    if(entry!=nullptr) {
        EntryDialog dialog(mDatabase.get());
        dialog.setEntry(entry);
        if(dialog.exec() == QDialog::Accepted) {
            //View should auto-rerequest data from model. If it doesnt, uncomment the following hack
            // int row = mDatabase->databaseContent().entries().indexOf(entry);
            // emit mEntriesModel.dataChanged(mEntriesModel.index(row,0),mEntriesModel.index(row,mEntriesModel.columnCount({})-1));

            //or implement update signals in entry class  and connect them to slot in model...
        }
        unsavedChanges = true;
    }
}

void MainWindow::deleteEntry(Entry* entry) {
    if(entry != nullptr) {
        if(QMessageBox::warning(this,"Really delete?", "Do you really want to permanently delete the entry '"+entry->title()+"' ?",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes) == QMessageBox::Yes) {
            mDatabase->databaseContent().removeEntry(entry);
            unsavedChanges = true;
        }
    }
}



void MainWindow::closeEvent(QCloseEvent *event)
{
    bool shouldCancel = closeDatabaseClicked();
    if(shouldCancel) {
        event->ignore();
    } else {
        QMainWindow::closeEvent(event);
    }
}
