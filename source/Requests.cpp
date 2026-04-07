#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#include <iostream>
#include <Requests.h>
#include <httplib.h>
#include <json.hpp>

static httplib::Client client("172.0.0.0.1", 8080);

std::string sendLogin(std::string username, std::string public_key, std::string password) {
    nlohmann::json request_body = {
        {"username", username},
        {"public_key", public_key},
    };

    if(password.size() != 0) request_body["password"] = password;

    auto result = client.Post("/login", request_body.dump(), "application/json");
    auto json_result = nlohmann::json::parse(result->body);
    std::cout << json_result.dump() << std::endl;

    return std::string("asbab");
}