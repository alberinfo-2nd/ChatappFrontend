#include "message.h"

Message::Message(const QString &timeStamp, const QString &sender, const QString &message)
    : m_timeStamp{timeStamp}
    , m_sender{sender}
    , m_message{message}
{}

QString Message::getSender() const{
    return m_sender;
}

QString Message::getMessage() const{
    return m_message;
}

QString Message::getTimeStamp() const{
    return m_timeStamp;
}