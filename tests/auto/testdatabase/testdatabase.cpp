#include "testdatabase.h"
#include <QTest>
#include <QRegularExpression>

#include "crypto/Crypto.h"
#include "database.h"

TestDatabase::TestDatabase(QObject *parent) : QObject(parent), factory(new TestFactory())
{

}

void TestDatabase::initTestCase()
{
    QVERIFY(Crypto::init());
}

void TestDatabase::testEmptyCreateLoad()
{
    std::unique_ptr<Database> d = Database::createNew("passw0rd");
    QVERIFY(d->write("test.db"));

    std::unique_ptr<Database> r = Database::createFromFile("test.db");
    QVERIFY(r->decrypt("passw0rd"));

    QTest::ignoreMessage(QtWarningMsg, QRegularExpression("Invalid padding.*"));
    QVERIFY(!r->decrypt("WRONGpassw0rd"));

}

void TestDatabase::testSingleEntryCreateLoad()
{
    std::unique_ptr<Database> d = Database::createNew("StrongPassw0rd");
    d->setFactory(factory);

    //Create single dummy entry
    QJsonObject obj1;
    obj1["a"]= 13;
    obj1["b"] = 3.141;
    obj1["c"] = "hallo";
    DummyEntry* dummy1 = new DummyEntry(factory);
    dummy1->jsonobject = obj1;

    //Append dummy entry to Database content
    QVERIFY(d->databaseContent().entries().isEmpty());
    d->databaseContent().addEntry(dummy1);
    QCOMPARE(d->databaseContent().entries().length(),1);
    QCOMPARE(d->databaseContent().entries().first(),dummy1);

    //Write Db.
    QVERIFY(d->write("test.db"));

    //Read db back in
    std::unique_ptr<Database> r = Database::createFromFile("test.db");
    r->setFactory(factory);
    QVERIFY(r->decrypt("StrongPassw0rd"));

    //Check that data was correctly restored
    QCOMPARE(r->databaseContent().entries().length(),1);
    Entry* entryRestored = r->databaseContent().entries().first();
    DummyEntry* dummyEntryRestored = static_cast<DummyEntry*>(entryRestored);
    QCOMPARE(dummyEntryRestored->jsonobject,obj1);

    //Now decrypt with wrong password and check if db content is empty
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression("Invalid padding.*"));
    QVERIFY(!r->decrypt("WRONGpassw0rd"));
    QVERIFY(r->databaseContent().entries().isEmpty());


}

void TestDatabase::testMultiEntryCreateLoad()
{
    std::unique_ptr<Database> d  = Database::createNew("StrongPassw0rd");
    d->setFactory(factory);
    QVERIFY(d->databaseContent().entries().isEmpty());

    //Create multiple dummy entries
    QVector<DummyEntry*> dummies;
    for(int i=0; i<10; i++) {
        QJsonObject obj;
        obj["a"]= qrand();
        obj["b"] = qrand()/13.9+qrand();
        DummyEntry* dummy = new DummyEntry(factory);
        dummy->jsonobject = obj;
        dummies.append(dummy);
        d->databaseContent().addEntry(dummy);
        QCOMPARE(d->databaseContent().entries().length(),i+1);
        QVERIFY(d->databaseContent().entries().contains(dummy));
    }

    //Write Db.
    QVERIFY(d->write("test.db"));

    //Read db back in
    std::unique_ptr<Database> r  = Database::createFromFile("test.db");
    r->setFactory(factory);
    QVERIFY(r->decrypt("StrongPassw0rd"));


    //Check that data was correctly restored
    QCOMPARE(r->databaseContent().entries().length(),dummies.length());
    for(int i=0; i<dummies.length(); i++) {
       Entry* entryRestored = r->databaseContent().entries()[i];
       DummyEntry* dummyEntryRestored = static_cast<DummyEntry*>(entryRestored);
       DummyEntry* dummyOriginal = dummies[i];
       QCOMPARE(dummyEntryRestored->jsonobject,dummyOriginal->jsonobject);

    }

    //Now decrypt with wrong password and check if db content is empty
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression("Invalid padding.*"));
    QVERIFY(!r->decrypt("WRONGpassw0rd"));
    QVERIFY(r->databaseContent().entries().isEmpty());

}

QTEST_APPLESS_MAIN(TestDatabase)
