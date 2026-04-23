#include "SessionManager.h"
// Used to manage the currently logged in user that the front end can keep track of the user's pub key, username, and auth token
// Constructor
SessionManager::SessionManager(QObject *parent)
    : QObject(parent)
{
}

// set current logged in user's data/info
void SessionManager::setCurrentUser(const QString &username, const QString &publicKey, const QByteArray &privateKey, const QString &authorizationToken)
{
    m_currentUser.setUsername(username);
    m_currentUser.setPublicKey(publicKey);
    m_privateKey = privateKey;
    m_authorizationToken = authorizationToken;
}

// Switch to new chat
void SessionManager::switchToNewChat(const QString &username, const QString &publicKey)
{
    m_currentPartnerName = username;

    // Check if this user is already reported
    bool isReported = (m_reportedUsers.count(username.toStdString()) > 0);
    emit chatSessionChanged(username, publicKey, isReported);
}

// Records reported user to maintain the UI state across chat sessions
void SessionManager::reportUser(const QString &username)
{
    m_reportedUsers.insert(username.toStdString());
}

// Clear current session
void SessionManager::clear()
{
    m_currentUser.setUsername("");
    m_currentUser.setPublicKey("");
    m_authorizationToken = "";
    m_privateKey.clear();
    m_inbox.clear();
    m_reportedUsers.clear();
    m_isAdmin = false;
}

// Set as admin
void SessionManager::setAsAdmin()
{
    m_isAdmin = true;
}

// Get username
QString SessionManager::getUsername() const
{
    return m_currentUser.getUsername();
}

// Get pub key
QString SessionManager::getPublicKey() const
{
    return m_currentUser.getPublicKey();
}

// Get auth token
QString SessionManager::getAuthorizationToken() const
{
    return m_authorizationToken;
}

QByteArray SessionManager::getPrivateKey() const
{
    return m_privateKey;
}

// Get is logged in user an admin (returns true or false)
bool SessionManager::getIsAdmin() const
{
    return m_isAdmin;
}

// Add messages to logged in user's inbox
void SessionManager::addMessages(const std::vector<Message> &messages)
{
    for (const auto &message : messages)
        m_inbox.push_back(message);

    inboxUpdated();
}

// remove message from logged in user's inbox
void SessionManager::removeMessage(size_t &index)
{
    if (m_inbox.empty())
    {
        return;
    }
    m_inbox.erase(m_inbox.begin() + index);
}

// get logged in user's inbox
const std::vector<Message> &SessionManager::getInbox() const
{
    return m_inbox;
}