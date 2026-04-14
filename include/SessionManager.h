#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QObject>
#include "User.h"
#include <vector>
#include <message.h>

class SessionManager : public QObject{

    Q_OBJECT

public:
    // deafault constructor for now plan to change this
    // TODO
    explicit SessionManager(QObject *parent = nullptr);

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

    bool getIsAdmin();

    void addMessages(const std::vector<Message> &messages);

    void removeMessage(size_t &index);

    const std::vector<Message>& getInbox() const;

signals:
    void inboxUpdated();

private:
    User m_currentUser{};
    QString m_authorizationToken;
    bool m_isAdmin{false};
    std::vector<Message> m_inbox{};
};

#endif // SESSIONMANAGER_H
