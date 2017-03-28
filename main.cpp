#include "mainwindow.h"
#include <QApplication>
#include "database.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Database d = Database::create_new("gagu", 20);
    d.write("test.db");

    Database r = Database::create_from_file("test.db");

    return a.exec();
}
