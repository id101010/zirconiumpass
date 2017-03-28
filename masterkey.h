#ifndef MASTERKEY_H
#define MASTERKEY_H

#include <QByteArray>

class Masterkey
{
    public:
        Masterkey();
        QByteArray key;
        bool derive_key(QString password,
                        QByteArray transform_seed,
                        int transform_rounds,
                        QByteArray master_seed);
};

#endif // MASTERKEY_H
