#ifndef ACTIVEUSERSMANAGER_H
#define ACTIVEUSERSMANAGER_H

#include <QObject>
#include <vector>
#include "user.h"
/**************************************************************
Class manager for all of the current logged in users
used mainly for storing updated list of active users and
managing their usernames and public keys. Created as a QObject
so that the class class can emit and listen for signals needed
for updatiing active users list.
***************************************************************/

class ActiveUsersManager : public QObject {
    Q_OBJECT
public:
    // constructor for class
    explicit ActiveUsersManager(QObject *parent = nullptr);

    // set the active users array
    void setActiveUsers(const std::vector<User> &users);

    // clear everying in array
    void clearActiveUsers();

    // return a vector of the active users
    std::vector<User> getActiveUsers() const;
signals:
    /***********************************************************
    signal for user list page and chat page to listen for so
    that the page will keep an updated list of logged in users
    ************************************************************/
    void activeUsersUpdated();

private:
    // data member for storing an array of active users
    std::vector<User> m_activeUsers{};


};

#endif //
