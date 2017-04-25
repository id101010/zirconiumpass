#include "testdatabase.h"
#include <QTest>
#include <QRegularExpression>

#include "crypto/Crypto.h"
#include "database.h"

TestDatabase::TestDatabase(QObject *parent) : QObject(parent), factory(new TestDatabase::Factory())
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

    QTest::ignoreMessage(QtWarningMsg, QRegularExpression("Invalid padding.*"));
    QVERIFY(!r.decrypt("WRONGpassw0rd"));

}

void TestDatabase::testSingleEntryCreateLoad()
{
    Database d = Database::createNew("StrongPassw0rd");
    d.setFactory(factory);

    //Create single dummy entry
    QJsonObject obj1;
    obj1["a"]= 13;
    obj1["b"] = 3.141;
    obj1["c"] = "hallo";
    DummyEntry dummy1;
    dummy1.jsonobject = obj1;

    //Append dummy entry to Database content
    QVERIFY(d.databaseContent().entires().isEmpty());
    d.databaseContent().addEntry(&dummy1);
    QCOMPARE(d.databaseContent().entires().length(),1);
    QCOMPARE(d.databaseContent().entires().first(),&dummy1);

    //Write Db.
    QVERIFY(d.write("test.db"));

    //Read db back in
    Database r = Database::createFromFile("test.db");
    r.setFactory(factory);
    QVERIFY(r.decrypt("StrongPassw0rd"));

    //Check that data was correctly restored
    QCOMPARE(r.databaseContent().entires().length(),1);
    JsonSerializable* entryRestored = r.databaseContent().entires().first();
    DummyEntry* dummyEntryRestored = static_cast<DummyEntry*>(entryRestored);
    QCOMPARE(dummyEntryRestored->jsonobject,obj1);

    //Now decrypt with wrong password and check if db content is empty
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression("Invalid padding.*"));
    QVERIFY(!r.decrypt("WRONGpassw0rd"));
    QVERIFY(r.databaseContent().entires().isEmpty());

}

void TestDatabase::testMultiEntryCreateLoad()
{
    Database d = Database::createNew("StrongPassw0rd");
    d.setFactory(factory);
    QVERIFY(d.databaseContent().entires().isEmpty());

    //Create multiple dummy entries
    QVector<DummyEntry*> dummies;
    for(int i=0; i<10; i++) {
        QJsonObject obj;
        obj["a"]= qrand();
        obj["b"] = qrand()/13.9+qrand();
        DummyEntry* dummy = new DummyEntry();
        dummy->jsonobject = obj;
        dummies.append(dummy);
        d.databaseContent().addEntry(dummy);
        QCOMPARE(d.databaseContent().entires().length(),i+1);
        QVERIFY(d.databaseContent().entires().contains(dummy));
    }

    //Write Db.
    QVERIFY(d.write("test.db"));

    //Read db back in
    Database r = Database::createFromFile("test.db");
    r.setFactory(factory);
    QVERIFY(r.decrypt("StrongPassw0rd"));


    //Check that data was correctly restored
    QCOMPARE(r.databaseContent().entires().length(),dummies.length());
    for(int i=0; i<dummies.length(); i++) {
       JsonSerializable* entryRestored = r.databaseContent().entires()[i];
       DummyEntry* dummyEntryRestored = static_cast<DummyEntry*>(entryRestored);
       DummyEntry* dummyOriginal = dummies[i];
       QCOMPARE(dummyEntryRestored->jsonobject,dummyOriginal->jsonobject);

    }

    //Now decrypt with wrong password and check if db content is empty
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression("Invalid padding.*"));
    QVERIFY(!r.decrypt("WRONGpassw0rd"));
    QVERIFY(r.databaseContent().entires().isEmpty());

    qDeleteAll(dummies);

}

QTEST_APPLESS_MAIN(TestDatabase)
