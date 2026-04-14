#include <user.h>

User::User()
    : m_username{""}
    , m_publicKey{""}
{}

User::User(QString username, QString publicKey)
    : m_username{username}
    , m_publicKey{publicKey}
{}

void User::setUsername(const QString &username) {
    m_username = username;
}

void User::setPublicKey(const QString &publicKey) {
    m_publicKey = publicKey;
}

QString User::getUsername() const{
    return m_username;
}

QString User::getPublicKey() const{
    return m_publicKey;
}