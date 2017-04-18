#include <QApplication>

#include "mainwindow.h"
#include "database.h"
#include "databaseopendialog.h"
#include "crypto/Crypto.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    if(!Crypto::init()) {
        qCritical() << "Could not initialize crypto";
    }

    Database d = Database::createNew("passw0rd");
    Q_ASSERT(d.write("test.db"));
    qDebug() << "ok encrypt & write";

    Database r = Database::createFromFile("test.db");

    Q_ASSERT(r.decrypt("passw0rd"));
    qDebug() << "ok read & decrypt";




    //QApplication a(argc, argv);
    //MainWindow w;
    // w.show();

    //DatabaseOpenDialog dialog;
    //dialog.exec();
    //return a.exec();
}
