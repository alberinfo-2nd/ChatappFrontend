#ifndef USER_H
#define USER_H

#include <QString>

/****************************************
This will later be changed to a class
until then this works
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