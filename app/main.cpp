#include <QApplication>

#include "mainwindow.h"
#include "databaseopendialog.h"
#include "crypto/Crypto.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    if(!Crypto::init()) {
        qCritical() << "Could not initialize crypto";
    }

    //Tests have been moved to separate subproject.
    //Check the build/tests folder or activate the Plugin "AutoTest" in QtCreator (Help -> Plugins)


    QApplication a(argc, argv);
    //MainWindow w;
    // w.show();

    DatabaseOpenDialog dialog;
    dialog.exec();

   // return a.exec();
}
