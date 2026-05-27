#pragma once
#include <string>

struct httpResponse {
    int statusCode;
    std::string statusText, contentType, body;

    std::string build() const;
};