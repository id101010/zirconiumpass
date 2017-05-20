#ifndef DATABASE_H
#define DATABASE_H

#include "masterkey.h"
#include "databasecontent.h"
#include <QString>
#include <memory>





class Database
{
    public:
        static std::unique_ptr<Database> createFromFile(QString filename);
        static std::unique_ptr<Database> createNew(QString password, int rounds = 100000);
        bool decrypt(QString password);
        bool write(QString filename);

        DatabaseContent& databaseContent();
        const DatabaseContent& databaseContent() const;

        const QByteArray& protectedStreamKey() const;

        void setFactory(QSharedPointer<DatabaseFactory> factory);

    private:
        Database();
        bool encrypt();
        QByteArray mProtectedStreamKey;
        Masterkey mMasterKey;
        DatabaseContent mDatabaseContent;

};

#endif // DATABASE_H
