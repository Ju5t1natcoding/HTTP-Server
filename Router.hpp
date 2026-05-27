#pragma once
#include "httpRequest.hpp"
#include "httpResponse.hpp"
#include <unordered_map>
#include <functional>

class Router {
private:
    std::unordered_map<std::string, std::function<httpResponse(const httpRequest&)>> routes;

public:
    void get(const std::string& path, std::function<httpResponse(const httpRequest&)> handler);
    httpResponse handle(const httpRequest& req);
};