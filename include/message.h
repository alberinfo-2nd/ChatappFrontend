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
    QString getTimeStamp() const;

private:
    QString m_timeStamp;
    QString m_sender;
    QString m_message;
};

#endif // MESSAGE_H
