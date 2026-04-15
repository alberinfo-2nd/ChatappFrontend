#include <user.h>

// default constructor for a user, sets a user's info to empty strings
User::User()
    : m_username{""}
    , m_publicKey{""}
{}

// parameterized constructer, create a new user by passing their username and public key
User::User(QString username, QString publicKey)
    : m_username{username}
    , m_publicKey{publicKey}
{}

// set username
void User::setUsername(const QString &username) {
    m_username = username;
}

// set public key
void User::setPublicKey(const QString &publicKey) {
    m_publicKey = publicKey;
}

// get username
QString User::getUsername() const{
    return m_username;
}

// get public key
QString User::getPublicKey() const{
    return m_publicKey;
}