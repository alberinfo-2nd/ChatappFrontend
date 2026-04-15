#ifndef USER_H
#define USER_H

#include <QString>

/****************************************
User class for basic user, holds their
username and public key.
*****************************************/

class User {
public:
    //constructor
    //default
    User();
    //parameterized
    User(QString username, QString publicKey);

    //setters
    void setUsername(const QString &username);
    void setPublicKey(const QString &publicKey);

    //getters
    QString getUsername() const;
    QString getPublicKey() const;
private:
    QString m_username;
    QString m_publicKey;
};

#endif