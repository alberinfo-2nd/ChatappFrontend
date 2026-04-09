#include "ActiveUsersManager.h"

ActiveUsersManager::ActiveUsersManager(QObject *parent)
    : QObject(parent) {};

void ActiveUsersManager::setActiveUsers(const std::vector<ActiveUser> &users) {
    m_activeUsers = users;
    emit activeUsersUpdated();
}

void ActiveUsersManager::clearActiveUsers() {
    m_activeUsers ={};
}

std::vector<ActiveUser> ActiveUsersManager::getActiveUsers() const {
    return m_activeUsers;
}

