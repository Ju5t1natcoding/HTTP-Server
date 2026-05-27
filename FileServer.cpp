#include "FileServer.hpp"
#include <fstream>
#include <sstream>
#include <unordered_map>

static std::string getContentType(const std::string& path) {
    static const std::unordered_map<std::string, std::string> types = {
        {".html", "text/html"},
        {".css", "text/css"},
        {".js", "application/javascript"},
        {".txt", "text/plain"}
    };

    auto dot = path.find('.');

    if (dot != std::string::npos) {
        auto it = types.find(path.substr(dot));

        if (it != std::end(types)) {
            return it->second;
        }
    }

    return "application/octet-string";
}

httpResponse FileServer::serve(const std::string& path) {
    std::ifstream file("static" + path);
    httpResponse resp;

    if (!file.is_open()) {
        resp.statusCode = 404;
        resp.statusText = "Not Found";
        resp.contentType = "text/plain";
        resp.body = "404 - Not Found";
        return resp;
    }

    std::ostringstream ss;
    ss << file.rdbuf();

    resp.statusCode = 200;
    resp.statusText = "OK";
    resp.contentType = getContentType(path);
    resp.body = ss.str();
    return resp;
}