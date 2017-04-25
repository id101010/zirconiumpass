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

        bool encrypt(const QByteArray& iv, const QByteArray& content, QByteArray& outEncrypted) const;
        bool decrypt(const QByteArray& iv, const QByteArray& encryptedContent, QByteArray& outDecrypted) const;
    private:
        QByteArray mKey;
};

#endif // MASTERKEY_H
