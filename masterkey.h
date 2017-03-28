#ifndef MASTERKEY_H
#define MASTERKEY_H

#include <QByteArray>
#include <QString>

class Masterkey
{
    public:
        Masterkey();
        bool derive_key(QString password,
                        QByteArray transform_seed,
                        int transform_rounds,
                        QByteArray master_seed);
    private:
        QByteArray m_key;
};

#endif // MASTERKEY_H
