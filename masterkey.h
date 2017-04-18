#ifndef MASTERKEY_H
#define MASTERKEY_H

#include <QByteArray>
#include <QString>

class Masterkey
{
    public:
        Masterkey();
        bool deriveKey(QString password,
                        QByteArray transformSeed,
                        int transformRounds,
                        QByteArray masterSeed);
    private:
        QByteArray m_key;
};

#endif // MASTERKEY_H
