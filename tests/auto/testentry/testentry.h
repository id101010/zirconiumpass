#ifndef TESTENTRY_H
#define TESTENTRY_H

#include <QObject>
#include "entry.h"

class TestEntry : public QObject
{
    Q_OBJECT
public:
    explicit TestEntry(QObject *parent = 0);

private slots:
    void initTestCase();
    void testSinglePlainValue();
    void testSingleCryptedValue();
};

#endif // TESTENTRY_H
