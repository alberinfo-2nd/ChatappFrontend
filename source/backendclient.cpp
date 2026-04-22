#ifdef _WIN32
#define _WIN32_WINNT 0x0A00

#include <iostream>
#include <BackendClient.h>
#include <json.hpp>
#include <SessionManager.h>


// Constructor: most important added attribute is the pointer to the sessionManager
// Initializes the client and sets up timers for automatic data polling
BackendClient::BackendClient(QObject *parent, SessionManager *sessionManager)
    : QObject(parent)
    , m_client("10.100.130.195", 8080)
    , m_activeUsersTimer(new QTimer(this))
    , m_messagesTimer(new QTimer(this))
    , m_sessionManager{sessionManager}
{
    // Connect timers to trigger background requests for users and messages
    connect(m_activeUsersTimer, &QTimer::timeout, this, &BackendClient::requestActiveUsers);
    connect(m_messagesTimer, &QTimer::timeout, this, &BackendClient::requestMessages);
}

// POST Request for sending login info return authorization token
std::string BackendClient::sendLogin(std::string username, std::string public_key, std::string password) {
    nlohmann::json request_body = {
        {"username", username},
        {"public_key", public_key},
    };

    if(password.size() != 0) request_body["password"] = password;

    auto result = m_client.Post("/login", request_body.dump(), "application/json");

    if (!result) {
        std::cerr << "HTTP request failed\n";
        return "Request failed";
    }

    std::clog << "Server response body: " << result->body << std::endl;

    auto json_result = nlohmann::json::parse(result->body);

    if (json_result["status"] == "Failed") {
        return json_result["message"];
    }

    return json_result["authorization_token"];
}

// Notifies the server to terminate the current session
void BackendClient::logout(std::string username, std::string authorizationToken) {
    nlohmann::json request_body = {
        {"username", username}
    };

    httplib::Request req;
    req.method = "POST";
    req.path = "/logout";
    req.body = request_body.dump();
    req.headers = {
        {"Content-Type", "application/json"},
        {"authorizationToken", authorizationToken}
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

    std::clog << "Server response body: " << result->body << std::endl;
}

// Requests the list of online users from the server
void BackendClient::requestActiveUsers() {
    // Skip request if user isn't logged in (used for exit button)
    if (m_sessionManager->getUsername().isEmpty()||
        m_sessionManager->getAuthorizationToken().isEmpty()) {
        return;
    }
    nlohmann::json request_body = {
        {"username", m_sessionManager->getUsername().toStdString()}
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

    auto json_result = nlohmann::json::parse(result->body);
    if (result->status != 200) {
        std::cerr << "Request failed with status " << result->status << ": " << result->body << std::endl;

        if(json_result["message"] == "Invalid authorization token or username") {
            emit userKicked(true);
        }

        return;
    }

    std::vector<User> users;
    for (const auto& user : json_result["users"]) {
        users.push_back(User{
            QString::fromStdString(user.value("username", "")),
            QString::fromStdString(user.value("public_key", ""))
        });
    }

    emit activeUsersReceived(users);

    return;
}

// Start polling timer for both request messages and request active users(start timer)
void BackendClient::startPolling() {
    m_activeUsersTimer->start(2000);
    m_messagesTimer->start(2000);

    requestActiveUsers();
    QTimer::singleShot(1000, this, [this]() {
    requestMessages();
    m_messagesTimer->start(2000);
    });
}

// Stop polling (stop timer)
void BackendClient::stopPolling() {
    m_activeUsersTimer->stop();
    m_messagesTimer->stop();
}
// Sends a new chat message to a specific recipient via the backend
void BackendClient::sendMessage(const std::string &recipient, const std::string &message) {
    nlohmann::json request_body = {
        {"username", m_sessionManager->getUsername().toStdString()},
        {"message", message},
        {"recipient", recipient}

    };

    httplib::Request req;
    req.method = "POST";
    req.path = "/sendMessage";
    req.body = request_body.dump();
    req.headers = {
        {"Content-Type", "application/json"},
        {"authorizationToken", m_sessionManager->getAuthorizationToken().toStdString()}
    };

    auto result = m_client.send(req);

    if (result) {
        std::cerr << "Request sent. Status:" << result->status << std::endl;
        std::cerr << "Response body:" << result->body << std::endl;
    } else {
        std::cerr << "Failed to send request" << result->status << std::endl;
    }
}

// Sends a report request to the server for a specific user
void BackendClient::reportUser(const std::string &reportedUser){
    if(m_sessionManager->getUsername().isEmpty()) return;
    nlohmann::json request_body = {
        {"username", m_sessionManager->getUsername().toStdString()},
        {"reportedUser", reportedUser}
    };
    httplib::Request req;
    req.method = "POST";
    req.path = "/report";
    req.body = request_body.dump();
    req.headers = {
        {"Content-Type", "application/json"},
        {"authorizationToken", m_sessionManager->getAuthorizationToken().toStdString()}
    };
    auto result = m_client.send(req);

    std::clog << "Server response body: " << result->body << std::endl;

    if (!result){
        std::cerr<<"Could not reach server"<<std::endl;
        return;
    }
        if (!result->body.empty()) {
            try {
                auto json_res = nlohmann::json::parse(result->body);
            } catch (const nlohmann::json::parse_error& e) {
                std::cerr << "JSON Parse Error: " << e.what() << std::endl;
            }
        } else {
            std::clog << "Server returned an empty body with status: " << result->status << std::endl; }
}

// Gets new messages from the server and parses them into Message objects for the UI
void BackendClient::requestMessages() {
    if (m_sessionManager->getUsername().isEmpty() ||
        m_sessionManager->getAuthorizationToken().isEmpty()) {
        return;
    }

    nlohmann::json request_body = {
        {"username", m_sessionManager->getUsername().toStdString()}
    };

    httplib::Request req;
    req.method = "GET";
    req.path = "/readMessages";
    req.body = request_body.dump();
    req.headers = {
        {"Content-Type", "application/json"},
        {"authorizationToken", m_sessionManager->getAuthorizationToken().toStdString()}
    };

    auto result = m_client.send(req);

    if (!result) {
        std::cerr << "requestMessages: HTTP request failed\n";
        return;
    }

    if (result->status != 200) {
        std::cerr << "requestMessages failed with status "
                  << result->status << ": "
                  << result->body << '\n';
        return;
    }

    nlohmann::json json_result;
    try {
        json_result = nlohmann::json::parse(result->body);
    } catch (const std::exception& e) {
        std::cerr << "requestMessages: JSON parse failed: " << e.what() << '\n';
        return;
    }

    if (!json_result.contains("messages") || !json_result["messages"].is_array()) {
        return;
    }

    std::vector<Message> messages;
    for (const auto& jsonMessage : json_result["messages"]) {
        messages.emplace_back(
            QString::fromStdString(jsonMessage.value("timestamp", "")),
            QString::fromStdString(jsonMessage.value("sender", "")),
            QString::fromStdString(jsonMessage.value("content", ""))
            );
    }

    if (!messages.empty()) {
        emit messageReceived(messages);
    }
}

// Admin only: Sends a request to disconnect a specific user from the server
std::string BackendClient::kick(std::string username) {
    nlohmann::json request_body = {
        {"username", m_sessionManager->getUsername().toStdString()},
        {"kickedUser", username}
    };

    httplib::Request req;
    req.method = "POST";
    req.path = "/kick";
    req.body = request_body.dump();
    req.headers = {
        {"Content-Type", "application/json"},
        {"authorizationToken", m_sessionManager->getAuthorizationToken().toStdString()}
    };

    auto result = m_client.send(req);

    if (!result) {
        std::cerr << "HTTP request failed\n";
        return "Request failed";
    }

    std::clog << "Server response body: " << result->body << std::endl;

    auto json_result = nlohmann::json::parse(result->body);

    if (json_result["status"] == "Failed") {
        return json_result["message"];
    }

    return json_result["message"];
}

#endif