#ifndef REQUESTS_H
#define REQUESTS_H

#include <json.hpp>

std::string sendLogin(std::string username, std::string public_key, std::string password = "");

#endif // REQUESTS_H
