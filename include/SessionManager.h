#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QObject>
#include "User.h"
#include <vector>
#include <message.h>
#include <set>

/*****************************************
SessionManager class is used to manage
the current logged in user's data. Such as
their username, public key, authorization
token, and inbox. Is a QObject so object
can emit signals and listen for signals.
******************************************/

class SessionManager : public QObject{

    Q_OBJECT

public:
    // deafault constructor for now plan to change this
    explicit SessionManager(QObject *parent = nullptr);

    // set current user
    void setCurrentUser(const QString &username, const QString &publicKey, const QByteArray &privateKey, const QString &authorizationToken);

    // set as admin
    void setAsAdmin();

    // clear
    void clear();

    // get username
    QString getUsername() const;

    // get pub key
    QString getPublicKey() const;

    // get auth token
    QString getAuthorizationToken() const;

    QByteArray getPrivateKey() const;

    // is logged in user an admin (returns true or false)
    bool getIsAdmin() const;

    // add messages to logged in user's inbox
    void addMessages(const std::vector<Message> &messages);

    // remove a message from logged in user's inbox
    void removeMessage(size_t &index);

    // get logged in user's inbox
    const std::vector<Message>& getInbox() const;

    // switch user chat
    void switchToNewChat(const QString &username, const QString &publicKey);

    // Tracks reported users to disable their chat UI
    void reportUser(const QString &username);

signals:
    // signal for chat page and user list page, emits when logged in user gets new messages.
    void inboxUpdated();
    // Notifies UI to switch the active conversation view
    void chatSessionChanged(const QString &username, const QString &publicKey, bool isReported);

private:
    // data member for storing current user (their username and public key)
    User m_currentUser{};

    // holds current user's authorization token
    QString m_authorizationToken;
    // Name of the user currently being messaged
    QString m_currentPartnerName;

    QByteArray m_privateKey;

    // data member to store whether logged in user is admin (true or false)
    bool m_isAdmin{false};

    // logged in user's inbox (vector of Message objects)
    std::vector<Message> m_inbox{};
    // Set of usernames reported during this session
    std::set<std::string> m_reportedUsers;
};

#endif // SESSIONMANAGER_H
