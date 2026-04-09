#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QString>
#include "User.h"

class SessionManager {
public:
    // deafault constructor for now plan to change this
    // TODO
    SessionManager() = default;

    // set current user
    void setCurrentUser(const QString &username, const QString &public_key, const QString &authorizationToken);

    // set as admin
    void setAsAdmin();

    // clear
    void clear();

    // get username
    QString getUsername();

    // get pub key
    QString getPublicKey();

    // get auth token
    QString getAuthorizationToken();


private:
    User m_currentUser{};
    bool m_isAdmin{false};
};

#endif // SESSIONMANAGER_H
