#pragma once
#include "httpResponse.hpp"

class FileServer {
public:
    static httpResponse serve(const std::string& path);
};