#include "ActiveUsersManager.h"

ActiveUsersManager::ActiveUsersManager(QObject *parent)
    : QObject(parent) {};
// Updates the local list of online users and notifies the UI to refresh
void ActiveUsersManager::setActiveUsers(const std::vector<User> &users) {
    m_activeUsers = users;
    emit activeUsersUpdated();
}
// Resets the list of active users (used during logout)
void ActiveUsersManager::clearActiveUsers() {
    m_activeUsers ={};
    emit activeUsersUpdated();
}
// Returns the current list of online users
std::vector<User> ActiveUsersManager::getActiveUsers() const {
    return m_activeUsers;
}

