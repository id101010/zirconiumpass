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
    QSharedPointer<DatabaseFactory> factory;

private:
   class DummyEntry : public Entry {
   public:
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


   class Factory : public DatabaseFactory  {
   public:
       virtual Entry *createEntry() override {
           return new DummyEntry();
       }
   };

};

#endif // TESTDATABASE_H
