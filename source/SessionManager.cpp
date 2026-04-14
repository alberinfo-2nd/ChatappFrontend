#include "SessionManager.h"
/*
Used to manage the currently logged in user that the front end can keep track of the user's pub key, username, and auth token
TODO add admin implementation
*/
// constructor
SessionManager::SessionManager(QObject *parent)
    : QObject(parent)
{
}

void SessionManager::setCurrentUser(const QString &username, const QString &public_key, const QString &authoirizationToken) {
    m_currentUser.setUsername(username);
    m_currentUser.setPublicKey(public_key);
    m_authorizationToken = authoirizationToken;
}

// clear current session
void SessionManager::clear() {
    m_currentUser.setUsername("");
    m_currentUser.setPublicKey("");
    m_authorizationToken = "";
    m_isAdmin = false;
}

// set to admin
//TODO currently not used in anything
void SessionManager::setAsAdmin() {
    m_isAdmin = true;
}

// get username
QString SessionManager::getUsername() {
    return m_currentUser.getUsername();
}

// get pub key
QString SessionManager::getPublicKey() {
    return m_currentUser.getPublicKey();
}

// get auth token
QString SessionManager::getAuthorizationToken() {
    return m_authorizationToken;
}

bool SessionManager::getIsAdmin() {
    return m_isAdmin;
}

void SessionManager::addMessages(const std::vector<Message> &messages) {
    for (const auto &message : messages)
        m_inbox.push_back(message);

    inboxUpdated();
}

void SessionManager::removeMessage(size_t &index) {
    if (m_inbox.empty()) {
        return;
    }
    m_inbox.erase(m_inbox.begin() + index);
}

const std::vector<Message>& SessionManager::getInbox() const{
    return m_inbox;
}