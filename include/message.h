#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
// Class to represent a single chat message
class Message
{
public:
    Message(const QString &timeStamp, const QString &sender, const QString &message);
    // Getters
    QString getSender() const;
    QString getMessage() const;
    QString getAdminPublicKey() const;
    QString getTimeStamp() const;
    // Setter
    void setAdminPublicKey(const QString &publicKey);


private:
    QString m_timeStamp;
    QString m_sender;
    QString m_adminPublicKey{""};
    QString m_message;
};

#endif // MESSAGE_H
