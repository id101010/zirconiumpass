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
    void openDatabaseClicked();
    void closeDatabaseClicked();
    void createNewDatabaseClicked();

private:
    EntriesTableModel mEntriesModel;
    Ui::MainWindow *ui;
    std::unique_ptr<Database> mDatabase;

};

#endif // MAINWINDOW_H
