#include <QApplication>

#include "mainwindow.h"
#include "crypto/Crypto.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    if(!Crypto::init()) {
        qCritical() << "Could not initialize crypto";
    }

    //Tests have been moved to separate subproject.
    //Check the build/tests folder or activate the Plugin "AutoTest" in QtCreator (Help -> Plugins)

    //Needed for Qsettings
    QCoreApplication::setOrganizationName("BFH");
    QCoreApplication::setApplicationName("ZirconiumPass");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
