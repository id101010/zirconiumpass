#ifndef DATABASECONTENT_H
#define DATABASECONTENT_H

#include "masterkey.h"
#include <QVector>

class Entry;

class DatabaseContent
{
public:
    DatabaseContent();
    bool encrypt(const Masterkey& masterkey);
    bool decrypt(const Masterkey& masterkey);


    const QVector<Entry*>& entires() const;
    void addEntry(Entry* entry);
    void removeEntry(Entry* entry);





 private:
    QVector<Entry*> mEntries;
};

#endif // DATABASECONTENT_H
