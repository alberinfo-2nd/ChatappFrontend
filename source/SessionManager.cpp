#include "SessionManager.h"
/*
Used to manage the currently logged in user that the front end can keep track of the user's pub key, username, and auth token
TODO add admin implementation
*/
// constructor
void SessionManager::setCurrentUser(const QString &username, const QString &public_key, const QString &authoirizationToken) {
    m_currentUser.username = username;
    m_currentUser.public_key = public_key;
    m_currentUser.authorizationToken = authoirizationToken;
}

// clear current session
void SessionManager::clear() {
    m_currentUser = {};
}

// set to admin
//TODO currently not used in anything
void SessionManager::setAsAdmin() {
    m_isAdmin = true;
}

// get username
QString SessionManager::getUsername() {
    return m_currentUser.username;
}

// get pub key
QString SessionManager::getPublicKey() {
    return m_currentUser.public_key;
}

// get auth token
QString SessionManager::getAuthorizationToken() {
    return m_currentUser.authorizationToken;
}