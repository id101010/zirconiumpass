#ifndef DATABASE_H
#define DATABASE_H

#include "masterkey.h"
#include <QString>

class Database
{
    public:
        Database();
        static Database create_from_file(QString filename);
        static Database create_new(QString password, int rounds);
        bool decrypt(QString password);
        bool write(QString filename);

    private:
        bool encrypt();
        QByteArray m_transform_seed;
        int m_transform_rounds;
        QByteArray m_master_seed;
        QByteArray m_encryption_iv;
        QByteArray m_stream_start_bytes;
        QByteArray m_protected_stream_key;
        Masterkey m_master_key;
};

#endif // DATABASE_H
