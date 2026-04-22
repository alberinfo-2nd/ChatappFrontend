#ifndef BACKENDCLIENT_H
#define BACKENDCLIENT_H

#include <QObject>
#include <string>
#include <vector>
#include <httplib.h>
#include <QTimer>
#include "user.h"
#include "message.h"
/************************************************************************************
manager for connecting to the backend and sending and recieving requests.
Becomes a QObject so that the class can emit signals, also has a timer
for /active-users to be requested every 5000ms(5s) which in return emits a signal
to the activeUserManager so the class knows to update the active user list with the
newly recieved list.
***************************************************************************************/

class SessionManager;

class BackendClient : public QObject {
    Q_OBJECT

public:
    // constructor for backend client pass thru parent and a pointer to the sessionManager object
    BackendClient(QObject *parent = nullptr, SessionManager *sessionManager = nullptr);

    // POST request for sending the login info to the backend returns authorization token
    std::string sendLogin(std::string username, std::string public_key, std::string password = "");

    void logout();

    // GET request for requesting active users emits activeUsersReceived if succesful which is connected
    // to setActiveUsers function keeping the list up to date
    void requestActiveUsers();

    // used to start polling active users (sets the timer to 5 seconds)
    void startPolling();

    // turns timer off
    void stopPolling();

    void sendMessage(const std::string &recipient, const std::string &message);

    void requestMessages();

    // used for the reported users
    void reportUser(const std::string &reportedUser);

    std::string kick(std::string username);

signals:
    // signal used to send activeUserManager the newly updated list of atcive users
    void activeUsersReceived(const std::vector<User> &users);
    // signal for updating message inbox
    void messageReceived(const std::vector<Message> &messages);

    void userKicked(bool kicked);

private:
    // data members
    httplib::Client m_client;
    SessionManager* m_sessionManager;
    QTimer* m_activeUsersTimer;
    QTimer* m_messagesTimer;
};

#endif // BACKENDCLIENT_H
