#include "httpResponse.hpp"

std::string httpResponse::build() const {
    std::string resp;
    resp += "HTTP/1.1 " + std::to_string(statusCode) + " " + statusText + "\r\n";
    resp += "Content-Type: " + contentType + "\r\n";
    resp += "Content-Length: " + std::to_string(static_cast<int>(body.size())) + "\r\n\r\n" + body;
    return resp;
}