#pragma once
#include <string>

struct httpRequest {
    std::string method, path, httpVersion;

    static httpRequest parse(const std::string& raw);
};