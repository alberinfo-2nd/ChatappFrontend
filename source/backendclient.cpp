#ifdef _WIN32
#define _WIN32_WINNT 0x0A00

#include <iostream>
#include <BackendClient.h>
#include <json.hpp>
#include <SessionManager.h>

// constructor most important added attribute is the pointer to the sessionManager
BackendClient::BackendClient(QObject *parent, SessionManager *sessionManager)
    : QObject(parent)
    , m_client("127.0.0.1", 8080)
    , m_activeUsersTimer(new QTimer(this))
    , m_sessionManager{sessionManager}
{
    // connect the timer to the request active users function
    connect(m_activeUsersTimer, &QTimer::timeout, this, &BackendClient::requestActiveUsers);
}

// POST Request for sending login info return authorization token
// TODO REWORK THIS FUNCTION TO HANDE ADMINS
std::string BackendClient::sendLogin(std::string username, std::string public_key, std::string password) {
    // body
    nlohmann::json request_body = {
        {"username", username},
        {"public_key", public_key},
    };

    if(password.size() != 0) request_body["password"] = password;

    auto result = m_client.Post("/login", request_body.dump(), "application/json");

    if (!result) {
        std::cerr << "HTTP request failed\n";
        return "";
    }
    auto json_result = nlohmann::json::parse(result->body);

    if (!json_result.contains("authorization_token") ||
        json_result["authorization_token"].is_null()) {
        std::cerr << "Login failed: no authorizationToken in response\n";
        return "";
    }

    return json_result["authorization_token"];
}
// request active users
void BackendClient::requestActiveUsers() {
    nlohmann::json request_body = {
        {"username", m_sessionManager->getUsername().toStdString()},
        {"public_key", m_sessionManager->getPublicKey().toStdString()}
    };

    httplib::Request req;
    req.method = "GET";
    req.path = "/active-users";
    req.body = request_body.dump();
    req.headers = {
        {"Content-Type", "application/json"},
        {"authorizationToken", m_sessionManager->getAuthorizationToken().toStdString()}
    };

    auto result = m_client.send(req);

    if (!result) {
        std::cerr << "HTTP request failed\n";
        return;
    }

    if (result->status != 200) {
        std::cerr << "Request failed with status " << result->status << ": " << result->body << std::endl;
        return;
    }

    auto json_result = nlohmann::json::parse(result->body);

    std::vector<ActiveUser> users;
    for (const auto& user : json_result["users"]) {
        users.push_back({
            QString::fromStdString(user.value("username", "")),
            QString::fromStdString(user.value("public_key", ""))
        });
    }

    if (!users.empty()) {
        emit activeUsersReceived(users);
    }
    return;
}

// start active user polling (start timer)
void BackendClient::startActiveUserPolling(int interval) {
    m_activeUsersTimer->start(interval);
}

// stop active user polling (stop timer)
void BackendClient::stopActiveUserPolling() {
    m_activeUsersTimer->stop();
}

#endif