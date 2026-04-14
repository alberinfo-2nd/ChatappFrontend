#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>

class Message
{
public:
    Message(const QString &timeStamp, const QString &sender, const QString &message);

    QString getSender() const;
    QString getMessage() const;
    QString getTimeStamp() const;

private:
    QString m_timeStamp;
    QString m_sender;
    QString m_message;
};

#endif // MESSAGE_H
