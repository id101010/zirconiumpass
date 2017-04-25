#include "testdatabase.h"
#include <QTest>

#include "crypto/Crypto.h"
#include "database.h"

TestDatabase::TestDatabase(QObject *parent) : QObject(parent)
{

}

void TestDatabase::initTestCase()
{
    QVERIFY(Crypto::init());
}

void TestDatabase::testEmptyCreateLoad()
{
    Database d = Database::createNew("passw0rd");
    QVERIFY(d.write("test.db"));

    Database r = Database::createFromFile("test.db");
    QVERIFY(r.decrypt("passw0rd"));

}

QTEST_APPLESS_MAIN(TestDatabase)
