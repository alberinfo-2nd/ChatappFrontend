#ifndef USER_H
#define USER_H

#pragma once
#include <string>

// User class for storing username, authorization token, and public key
class User {
private:
    std::string m_username;
    std::string m_authorizationToken;
    std::string m_publicKey;

public:
    //constructor
    User(std::string username, std::string authorizationToken, std::string publicKey);

    // getter functions
    std::string getUsername();
    std::string getAuthorizationToken();
    std::string getPublicKey();
};

#endif // USER_H