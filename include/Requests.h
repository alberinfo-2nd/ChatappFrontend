#ifndef REQUESTS_H
#define REQUESTS_H

#pragma once

#include <string>
#include <vector>

std::string sendLogin(std::string username, std::string public_key, std::string password = "");

const std::vector<std::string> requestActiveUsers();

#endif // REQUESTS_H
