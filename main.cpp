#include "mainwindow.h"
#include <QApplication>
#include "database.h"
#include "databaseopendialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    DatabaseOpenDialog dialog;

    dialog.show();
    //w.show();

    Database d = Database::create_new("test", 20);
    d.write("test.db");

    Database r = Database::create_from_file("test.db");

    return a.exec();
}
