#ifndef DATABASE_H
#define DATABASE_H

#include "masterkey.h"
#include <QString>

class Database
{
    public:
        static Database createFromFile(QString filename);
        static Database createNew(QString password, int rounds = 100000);
        bool decrypt(QString password);
        bool write(QString filename);

    private:
        Database();
        bool encrypt();
        QByteArray mTransformSeed;
        QByteArray mMasterSeed;
        QByteArray mEncryptionIv;
        QByteArray mStreamStartBytes;
        QByteArray mProtectedStreamKey;
        int mTransformRounds;
        Masterkey mMasterKey;
};

#endif // DATABASE_H
