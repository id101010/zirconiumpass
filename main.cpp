#include <QApplication>

#include "mainwindow.h"
#include "database.h"
#include "databaseopendialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    DatabaseOpenDialog dialog;

    dialog.exec();
    //w.show();

    Database d = Database::createNew("test", 20);
    d.write("test.db");

    Database r = Database::createFromFile("test.db");

    return a.exec();
}
