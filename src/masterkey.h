#ifndef MASTERKEY_H
#define MASTERKEY_H

#include <QByteArray>
#include <QString>

/**
 * @brief The Masterkey class provides key-derivation and encrypt/decrypt facilities
 */
class Masterkey
{
    public:
        Masterkey();
        int transformRounds() const;
        void setTransformRounds(int rounds);

        void setTransformSeed(const QByteArray& seed);
        QByteArray transformSeed() const;

        void setMasterSeed(const QByteArray& seed);
        QByteArray masterSeed() const;

        bool deriveKey(QString password);

        bool encrypt(const QByteArray& iv, const QByteArray& content, QByteArray& outEncrypted) const;
        bool decrypt(const QByteArray& iv, const QByteArray& encryptedContent, QByteArray& outDecrypted) const;
    private:
        QByteArray mKey; //TODO: Memory protect this
        QByteArray mTransformSeed;
        QByteArray mMasterSeed;
        int mTransformRounds;
};

#endif // MASTERKEY_H
