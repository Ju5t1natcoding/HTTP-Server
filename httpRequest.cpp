#include "httpRequest.hpp"
#include <sstream>

httpRequest httpRequest::parse(const std::string& raw) {
    httpRequest req;
    std::istringstream is(raw);
    is >> req.method >> req.path >> req.httpVersion;
    return req;
}