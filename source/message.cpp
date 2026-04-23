#include "message.h"
// Constructor
Message::Message(const QString &timeStamp, const QString &sender, const QString &message)
    : m_timeStamp{timeStamp}
    , m_sender{sender}
    , m_message{message}
{}
// Getters
QString Message::getSender() const{
    return m_sender;
}

QString Message::getMessage() const{
    return m_message;
}

QString Message::getAdminPublicKey() const{
    return m_adminPublicKey;
}

QString Message::getTimeStamp() const{
    return m_timeStamp;
}

void Message::setAdminPublicKey(const QString &publicKey) {
    m_adminPublicKey = publicKey;
}