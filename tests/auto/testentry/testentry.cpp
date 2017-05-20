#include "testentry.h"
#include <QJsonArray>
#include <QTest>
#include <cryptedvalue.h>
#include <plainvalue.h>

TestEntry::TestEntry(QObject *parent) : QObject(parent)
{

}

void TestEntry::initTestCase()
{
}

void TestEntry::testSinglePlainValue()
{
    Entry e;
    e.setTitle("testplain");

    PlainValue *v = new PlainValue();
    v->setName("password1");
    v->setValue("passw0rd");
    e.addValue(v);

    QJsonObject entryJson = e.saveToJson();
    QCOMPARE(entryJson["title"].toString(),QString("testplain"));
    QCOMPARE(entryJson["values"].isArray(),true);
    QCOMPARE(entryJson.keys().count(),2);

    QJsonArray arr = entryJson["values"].toArray();

    QCOMPARE(arr.count(),1);
    QJsonObject valueJson = arr[0].toObject();
    QCOMPARE(valueJson.keys().count(),3);
    QCOMPARE(valueJson["type"].toString(),QString("plain"));
    QCOMPARE(valueJson["name"].toString(),QString("password1"));
    QCOMPARE(valueJson["plainValue"].toString(),QString("passw0rd"));
}

void TestEntry::testSingleCryptedValue()
{
    Entry e;
    e.setTitle("testcrypted");

    CryptedValue *v = new CryptedValue();
    v->setName("password2");
    v->setValue("gggggg", "secretpassw0rt");
    e.addValue(v);

    QJsonObject entryJson = e.saveToJson();
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

    QJsonArray arr2 = entryJson["cryptedValue"].toArray();
    QCOMPARE(arr2.count(), 17);

    for(int i=0; i<arr2.count(); i++){
        QJsonValue b = arr2.at(i);
        QCOMPARE(b.isDouble(), true);
        int x = b.toInt();
    }
}

QTEST_APPLESS_MAIN(TestEntry)
