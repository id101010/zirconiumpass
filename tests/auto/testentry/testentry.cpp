#include "testentry.h"
#include <QJsonArray>
#include <QTest>
#include <cryptedvalue.h>
#include <databasecontent.h>
#include <plainvalue.h>
#include "crypto/Random.h"
#include "crypto/Crypto.h"

TestEntry::TestEntry(QObject *parent) : QObject(parent)
{

}

void TestEntry::initTestCase()
{
    QVERIFY(Crypto::init());
}

void TestEntry::testSinglePlainValue()
{
    QSharedPointer<Factory> factory =  QSharedPointer<Factory>::create();

    /* ------------- Save ------------- */

    Entry saveEntry(factory);
    saveEntry.setTitle("testplain");

    PlainValue *v = new PlainValue();
    v->setName("password1");
    v->setValue("passw0rd");
    saveEntry.addValue(v);

    QJsonObject entryJson = saveEntry.saveToJson();
    QCOMPARE(entryJson["title"].toString(),QString("testplain"));
    QCOMPARE(entryJson["values"].isArray(),true);
    QCOMPARE(entryJson.keys().count(),2);

    QJsonArray arr = entryJson["values"].toArray();

    QCOMPARE(arr.count(), 1);
    QJsonObject valueJson = arr[0].toObject();
    QCOMPARE(valueJson.keys().count(),3);
    QCOMPARE(valueJson["type"].toString(),QString("plain"));
    QCOMPARE(valueJson["name"].toString(),QString("password1"));
    QCOMPARE(valueJson["plainValue"].toString(),QString("passw0rd"));

    /* ------------- Load ------------- */

    Entry loadEntry(factory);
    QCOMPARE(loadEntry.loadFromJson(entryJson), true);
    QCOMPARE(loadEntry.title(), saveEntry.title());
    QCOMPARE(loadEntry.values().count(), saveEntry.values().count());

    for(int i=0; i<loadEntry.values().count(); i++){
        AbstractValue *loaded = loadEntry.values()[i];
        AbstractValue *saved = saveEntry.values()[i];
        QCOMPARE(loaded->name(), saved->name());
        QCOMPARE(loaded->type(), saved->type());
        QCOMPARE(static_cast<PlainValue*>(loaded)->value(), static_cast<PlainValue*>(saved)->value());
    }
}

void TestEntry::testSingleCryptedValue()
{
    QSharedPointer<Factory> factory =  QSharedPointer<Factory>::create();

    /* ------------- Save ------------- */

    QByteArray streamKey = randomGen()->randomArray(32);
    QString pw = "secretpassw0rt";

    Entry saveEntry(factory);
    saveEntry.setTitle("testcrypted");

    CryptedValue *v = new CryptedValue();
    v->setName("password2");
    v->setValue(streamKey, pw);
    saveEntry.addValue(v);

    QJsonObject entryJson = saveEntry.saveToJson();
    QCOMPARE(entryJson["title"].toString(),QString("testcrypted"));
    QCOMPARE(entryJson["values"].isArray(),true);
    QCOMPARE(entryJson.keys().count(),2);

    QJsonArray arr = entryJson["values"].toArray();

    QCOMPARE(arr.count(),1);
    QJsonObject valueJson = arr[0].toObject();
    QCOMPARE(valueJson.keys().count(),3);
    QCOMPARE(valueJson["type"].toString(),QString("encrypted"));
    QCOMPARE(valueJson["name"].toString(),QString("password2"));

    QCOMPARE(valueJson["cryptedValue"].isArray(), true);

    QJsonArray arr2 = valueJson["cryptedValue"].toArray();
    QCOMPARE(arr2.count(), pw.length()+1);

    for(int i=0; i<arr2.count(); i++){
        QJsonValue b = arr2.at(i);
        QCOMPARE(b.isDouble(), true);

        //Checking the actual int values would require us to do the crypto manually here....
        //It should be enough if we test the load below
    }

    /* ------------- Load ------------- */

    Entry loadEntry(factory);
    QCOMPARE(loadEntry.loadFromJson(entryJson), true);
    QCOMPARE(loadEntry.title(), saveEntry.title());
    QCOMPARE(loadEntry.values().count(), saveEntry.values().count());

    for(int i=0; i<loadEntry.values().count(); i++){
        AbstractValue *loaded = loadEntry.values()[i];
        AbstractValue *saved = saveEntry.values()[i];
        QCOMPARE(loaded->name(), saved->name());
        QCOMPARE(loaded->type(), saved->type());
        bool visitorHasBeenCalled = false;
        static_cast<CryptedValue*>(loaded)->decrypt(streamKey,[&pw,&visitorHasBeenCalled](const QString& actualPw) {
            QCOMPARE(actualPw, pw);
            visitorHasBeenCalled  = true;
        });
        QCOMPARE(visitorHasBeenCalled, true);
    }
}

QTEST_APPLESS_MAIN(TestEntry)
