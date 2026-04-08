#include <user.h>

User::User(std::string username, std::string authorizationToken, std::string publicKey)
    : m_username{username}
    , m_authorizationToken{authorizationToken}
    , m_publicKey{publicKey}
{}

std::string User::getUsername() {
    return m_username;
}

std::string User::getAuthorizationToken() {
    return m_authorizationToken;
}

std::string User::getPublicKey() {
    return m_publicKey;
}