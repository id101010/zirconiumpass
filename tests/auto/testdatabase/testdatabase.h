#ifndef TESTDATABASE_H
#define TESTDATABASE_H

#include <QObject>
#include "databasecontent.h"




class TestDatabase : public QObject
{
    Q_OBJECT
public:
    explicit TestDatabase(QObject *parent = 0);

private slots:
    void initTestCase();
    void testEmptyCreateLoad();
    void testSingleEntryCreateLoad();
    void testMultiEntryCreateLoad();

private:
    QSharedPointer<Factory> factory;

private:
   class DummyEntry : public Entry {
   public:
       DummyEntry(QSharedPointer<Factory> factory) : Entry(factory){

       }

       virtual QJsonObject saveToJson() const override
       {
           return jsonobject;
       }
       virtual bool loadFromJson(const QJsonObject &obj) override
       {
           jsonobject = obj;
           return loadShouldReturnTrue;
       }
       QJsonObject jsonobject;
       bool loadShouldReturnTrue = true;
   };


   class TestFactory : public Factory {
   public:
       virtual Entry *createEntry() override {
           return new DummyEntry(this->sharedFromThis());
       }
   };

};

#endif // TESTDATABASE_H
