#ifndef TESTDATABASE_H
#define TESTDATABASE_H

#include <QObject>

class TestDatabase : public QObject
{
    Q_OBJECT
public:
    explicit TestDatabase(QObject *parent = 0);

private slots:
    void initTestCase();
    void testEmptyCreateLoad();
};

#endif // TESTDATABASE_H
