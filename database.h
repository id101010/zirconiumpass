#ifndef DATABASE_H
#define DATABASE_H

#include "masterkey.h"
#include <QString>

class Database
{
    public:
        static Database create_from_file(QString filename);
        static Database create_new(QString password, int rounds);
        bool decrypt(QString password);
        bool write(QString filename);

    private:
        Database();
        bool encrypt();
        QByteArray m_transform_seed;
        QByteArray m_master_seed;
        QByteArray m_encryption_iv;
        QByteArray m_stream_start_bytes;
        QByteArray m_protected_stream_key;
        int m_transform_rounds;
        Masterkey m_master_key;
};

#endif // DATABASE_H
