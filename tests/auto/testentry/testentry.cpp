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
    /* ------------- Save ------------- */

    Entry saveEntry;
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

    Entry loadEntry;
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
    /* ------------- Save ------------- */

    Entry saveEntry;
    saveEntry.setTitle("testcrypted");

    CryptedValue *v = new CryptedValue();
    v->setName("password2");
    v->setValue("gggggg", "secretpassw0rt");
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

    QJsonArray arr2 = entryJson["cryptedValue"].toArray();
    //QCOMPARE(arr2.count(), 17);

    for(int i=0; i<arr2.count(); i++){
        QJsonValue b = arr2.at(i);
        QCOMPARE(b.isDouble(), true);
        int x = b.toInt();
    }

    /* ------------- Load ------------- */

    Entry loadEntry;
    QCOMPARE(loadEntry.loadFromJson(entryJson), true);
    QCOMPARE(loadEntry.title(), saveEntry.title());
    QCOMPARE(loadEntry.values().count(), saveEntry.values().count());

    for(int i=0; i<loadEntry.values().count(); i++){
        AbstractValue *loaded = loadEntry.values()[i];
        AbstractValue *saved = saveEntry.values()[i];
        QCOMPARE(loaded->name(), saved->name());
        QCOMPARE(loaded->type(), saved->type());
        //QCOMPARE(static_cast<CryptedValue*>(loaded)->value(), static_cast<CryptedValue*>(saved)->value());
    }
}

QTEST_APPLESS_MAIN(TestEntry)
