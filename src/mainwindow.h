#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "database.h"
#include "entriestablemodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    //Menu action slots
    void openDatabaseClicked();
    bool closeDatabaseClicked();
    void createNewDatabaseClicked();
    void createNewEntryClicked();
    void saveDatabaseClicked();
    void editEntryClicked();
    void deleteEntryClicked();

    void tableContextMenuRequested(const QPoint &pos);
    void entryDoubleClicked(const QModelIndex& index);
    void tableHeaderContextMenuRequested(const QPoint &pos);
    void selectionChanged(const class QItemSelection& selected);

private:
    Ui::MainWindow *ui;
    bool unsavedChanges;
    EntriesTableModel mEntriesModel;
    std::unique_ptr<Database> mDatabase;

    void editEntry(Entry* entry);
    void deleteEntry(Entry *entry);
protected:
    virtual void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
